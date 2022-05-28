// -*- coding: utf-8-unix -*-
/**
 * \file sound.c
 *
 * Copyright (c) 2021 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

#include "../include/bios.h"
#include "../include/psg.h"
#include "../include/sound.h"
#include "../include/sound_eg.h"

#include "sound_eg_spi.h"

#include "../include/ay_3_8910.h"

#define DI()                __asm__("di")
#define EI()                __asm__("ei")

#define PSG_SET(reg, val)   ay_3_8910_buffer[(reg)] = (val)
#define PSG_GET(reg)        ay_3_8910_buffer[(reg)]

#define COUNT_PER_SECOND    (300)
#define COUNT_PER_TICK_60HZ (COUNT_PER_SECOND / 60)

static uint8_t COUNT_PER_TICK;

struct sound_channel {
  /* sequence control */
  const uint8_t* next;          // pointer to next chunk for each streams
  int16_t duration;             // remaining duration for each channel
  /* envelope generator */
  struct sound_eg eg;
  /* amplifier */
  uint8_t amp;                  // 4 bits
  /* Whether use the hardware envelope generator is enabled or not. */
  bool hw_envelope_enable;
};

struct sound_state {
  const struct sound_clip* clip; // pointer to current music (set of streams)
  struct sound_channel channels[3];
  uint8_t section;              // current fragment number
  uint8_t flag;                 // playing/stopped flags
  uint8_t envelope_pattern;     // R#13 hardware envelope pattern (4 bits)
  uint8_t count_per_tick;
};

static struct {
  bool repeat;                // auto repeat on/off
  bool paused;                // whether the BGM is paused or not.
  uint8_t volume;             // volume control (4 bits)
  struct sound_state bg;      // state of background music
  struct sound_state se;      // state of sound effect
} sound;

// -----

static const struct sound_eg_AHDSR sound_eg_AHDSR_DEFAULT = {
  .attack_rate = 255,
  .hold_time = 0,
  .decay_rate = 12,
  .sustain_level = 0,
  .release_rate = 255,
};

static const uint8_t lpcm_default[] = {255};
static const struct sound_eg_LPCM sound_eg_LPCM_DEFAULT = {
  .len = sizeof(lpcm_default),
  .data = lpcm_default,
};

static const struct sound_eg_attribute sound_eg_table_default[] = {
  [0] = {.clz = SOUND_EG_AHDSR, .param = &sound_eg_AHDSR_DEFAULT},
  [1] = {.clz = SOUND_EG_LPCM , .param = &sound_eg_LPCM_DEFAULT },
};

static const struct sound_eg_attribute * sound_eg_table;

uint8_t sound_tick;
uint8_t sound_advance_count;

extern const struct sound_eg_vtbl sound_eg_AHDSR;
extern const struct sound_eg_vtbl sound_eg_LPCM;

static const struct sound_eg_vtbl * sound_eg_classes[] = {
  [SOUND_EG_AHDSR] = &sound_eg_AHDSR,
  [SOUND_EG_LPCM] = &sound_eg_LPCM,
};

inline void sound_eg_key_on(struct sound_eg * self) {
  sound_eg_classes[self->attr.clz]->key_on(self);
}

inline void sound_eg_key_off(struct sound_eg * self) {
  sound_eg_classes[self->attr.clz]->key_off(self);
}

inline void sound_eg_advance(struct sound_eg * self) {
  for (uint8_t i = 0; i < sound_advance_count; ++i) {
    sound_eg_classes[self->attr.clz]->advance(self);
  }
}

inline void sound_update_tick(void) {
  sound_tick += COUNT_PER_TICK;
  sound_advance_count = 0;
  while (COUNT_PER_TICK_60HZ <= sound_tick) {
    sound_tick -= COUNT_PER_TICK_60HZ;
    sound_advance_count++;
  }
}

// -----
static void sound_set_eg_table0(const struct sound_eg_attribute * table);
static void sound_set_speed0(uint8_t multiplier);
static void sound_set_repeat0(bool repeat);
static void sound_set_volume0(uint8_t volume);
static void sound_set_mute0(uint8_t mute);
static void sound_effect0(const struct sound_clip* s);
static void sound_set_bgm0(const struct sound_clip* s);
static void sound_start0(void);
static void sound_init0(void);
static void sound_stop0(void);
static void sound_pause0(void);

// -----
static void sound_set_eg_table0(const struct sound_eg_attribute * table) {
  sound_eg_table = table ? table : sound_eg_table_default;
}

static void sound_channel_set_envelope(struct sound_channel * sc, uint8_t index) {
  sc->eg.attr.clz = sound_eg_table[index].clz;
  sc->eg.attr.param = sound_eg_table[index].param;
  sc->eg.output = 0;
  sc->eg.state = 0;
  sc->eg.time = 0;
  sc->hw_envelope_enable = false;
}

inline void sound_channel_set_amp(struct sound_channel * sc, uint8_t amp) {
  sc->amp = amp;
}

inline void sound_channel_enable_hw_envelope(struct sound_channel * sc) {
  sc->hw_envelope_enable = true;
}

inline void sound_channel_reset(struct sound_channel * sc) {
  sound_channel_set_amp(sc, 0);
  sound_channel_set_envelope(sc, 0);
}

// -----

static void sound_set_speed0(uint8_t multiplier) {
  if (multiplier < SOUND_SPEED_MIN) {
    multiplier = SOUND_SPEED_MIN;
  }
  if (SOUND_SPEED_MAX < multiplier) {
    multiplier = SOUND_SPEED_MAX;
  }
  sound.bg.count_per_tick = multiplier;
}

static void sound_set_repeat0(bool repeat) {
  sound.repeat = repeat;
}

static struct {
  uint8_t noise_fdr;
  uint8_t mixer;
  uint8_t envelope_cycle_lo;
  uint8_t envelope_cycle_hi;
} backup;

static void sound_backup_psg_registers(void) {
  backup.noise_fdr = PSG_GET(6);
  backup.mixer = PSG_GET(7);
  backup.envelope_cycle_lo = PSG_GET(11);
  backup.envelope_cycle_hi = PSG_GET(12);
}

static void sound_restore_psg_registers(void) {
  sound.bg.channels[0].eg.output = 0;
  sound.bg.channels[1].eg.output = 0;
  sound.bg.channels[2].eg.output = 0;
  PSG_SET(6, backup.noise_fdr);
  PSG_SET(7, backup.mixer);
  PSG_SET(8, 0);
  PSG_SET(9, 0);
  PSG_SET(10, 0);
  PSG_SET(11, backup.envelope_cycle_lo);
  PSG_SET(12, backup.envelope_cycle_hi);
}

static void sound_set_volume0(uint8_t volume) {
  sound.volume = (15 < volume ? 15 : volume);
}

static void sound_set_mute0(uint8_t mute) {
  mute &= 7;
  sound.bg.flag &= ~(7 << 3);
  sound.bg.flag |= mute << 3;
  sound.se.flag &= ~(7 << 3);
  sound.se.flag |= mute << 3;
  if (mute & SOUND_CHANNEL_A) psg_set(8, 0);
  if (mute & SOUND_CHANNEL_B) psg_set(9, 0);
  if (mute & SOUND_CHANNEL_C) psg_set(10, 0);
}

static void sound_set_fragment(struct sound_state* st, const struct sound_fragment* sf) {
  uint8_t flag = st->flag & ~SOUND_CHANNEL_ALL;
  st->channels[0].next = sf->streams[0];
  st->channels[1].next = sf->streams[1];
  st->channels[2].next = sf->streams[2];
  st->channels[0].duration = 0;
  st->channels[1].duration = 0;
  st->channels[2].duration = 0;
  if (sf->streams[0] && *sf->streams[0] != 0xff) flag |= SOUND_CHANNEL_A;
  if (sf->streams[1] && *sf->streams[1] != 0xff) flag |= SOUND_CHANNEL_B;
  if (sf->streams[2] && *sf->streams[2] != 0xff) flag |= SOUND_CHANNEL_C;
  st->flag = flag;
}

static void sound_state_init(struct sound_state * st);

static void sound_set_clip(struct sound_state* st, const struct sound_clip* s) {
  sound_state_init(st);
  if (!s || !s->fragments || 0 == s->num_fragments) {
    return;
  }
  const struct sound_fragment* sf = s->fragments[0];
  sound_set_fragment(st, sf);
  st->clip = s;
}

static volatile const struct sound_clip * next_sound_effect;
static void sound_effect0(const struct sound_clip* s) {
  next_sound_effect = s;
}

static void sound_effect_apply(void) {
  const struct sound_clip * s = next_sound_effect;
  next_sound_effect = NULL;
  if (!s) return;
  if (sound.se.clip) {
    if (sound.se.flag & SOUND_CHANNEL_ALL) {
      if (s->priority < sound.se.clip->priority) {
        return;
      }
    }
    // When switching sound effects, using channels may differ between previous
    // and next sound effects, so temporally restores register values for BGM
    // before playing the next sound effects.
    sound_restore_psg_registers();
  }
  else {
    sound_backup_psg_registers();
  }
  sound_set_clip(&sound.se, s);
}

static void sound_set_bgm0(const struct sound_clip* s) {
  sound_stop0();
  if (!s) {
    return;
  }
  sound_set_clip(&sound.bg, s);
}

static void sound_start0(void) {
  sound.paused = false;
}

static void sound_state_init(struct sound_state * st) {
  st->clip = 0;
  st->flag = 0;
  st->section = 0;
  // ----
  for (uint8_t ch = 0; ch < 3; ++ch) {
    sound_channel_reset(&st->channels[ch]);
  }
  st->envelope_pattern = 0;
}

static void sound_init0(void) {
  COUNT_PER_TICK = COUNT_PER_SECOND / msx_get_vsync_frequency();
  sound.bg.count_per_tick = SOUND_SPEED_1X;
  sound.se.count_per_tick = SOUND_SPEED_1X;
  sound_eg_table = sound_eg_table_default;
  sound_set_volume0(15);
  sound_stop0();
}

static void sound_stop0(void) {
  sound_effect0(NULL);
  sound_pause0();
  ay_3_8910_init();
  sound_state_init(&sound.bg);
  sound_state_init(&sound.se);
  sound_tick = 0;
  sound_advance_count = 0;
}

static void sound_pause0(void) {
  sound.paused = true;
  psg_set(8, 0);
  psg_set(9, 0);
  psg_set(10, 0);
}

// ---- local work area for `sound_player()` ----
static struct {
  // -- context for current processing sound_state
  struct sound_state * st;
  uint8_t mute;
  // -- context for current processing sound_channel
  uint8_t ch;
  uint8_t mask;
  bool channel_muted;
  struct sound_channel * stch;
} sound_player_ctx;

#define ctx sound_player_ctx

static void sound_channel_output(void) {
  int8_t x = (ctx.stch->eg.output >> 4) + ctx.stch->amp + sound.volume - 30;
  if (x < 0) {
    x = 0;
  }
  PSG_SET(8+ctx.ch, x);
  // advance the channel's envelope generator by 1/60th a second.
  sound_eg_advance(&ctx.stch->eg);
}

static void sound_player__process_channel(void) {
  // ---- Reads head (chunk size and next duration) ----
  uint8_t len;
  const uint8_t * next = ctx.stch->next;
  {
    const uint8_t head1 = *next++;
    if (head1 == 255) {
      // end of music track
      ctx.st->flag &= ~ctx.mask;
      return;
    }
    const uint8_t ticks_lo = *next++;
    const uint8_t ticks_hi = (head1 & 0x1f);
    const uint16_t ticks = (ticks_hi << 8) | (ticks_lo);
    ctx.stch->duration += SOUND_SPEED_1X * ticks;
    len = (head1 >> 5) & 7;
    ctx.stch->next = next + len;
    if (ctx.channel_muted) {
      return;
    }
  }
  // ---- Parse and process the chunk ----
  while (len--) {
    const uint8_t x = *next++;
    switch (x >> 4) {
    case 0x0:
      // keyon (12bits)
      // 0000b fdr_hi:4 fdr_lo:8
      // -- Changes frequency of tone generator and performs a key-on.
      sound_eg_key_on(&ctx.stch->eg);
      // When using hardware envelopes, special processing is required.
      if (ctx.stch->hw_envelope_enable) {
        PSG_SET(13, ctx.st->envelope_pattern);
        PSG_SET(8+ctx.ch, 16);
      }
      // [[FALL THROUGH]]
    case 0xe:
      // tone (12bits)
      // 1110b fdr_hi:4 fdr_lo:8
      // -- changes frequency of tone generator but do not key-on.
      {
        const uint8_t fdr_hi = x & 0x0f;
        const uint8_t fdr_lo = *next++;
        len--;
        PSG_SET(1+2*ctx.ch, fdr_hi); // fdr_hi:4
        PSG_SET(0+2*ctx.ch, fdr_lo); // fdr_lo:8
      }
      break;
    case 0x1:
      // keyoff (0bits)
      // -- Perform a key-off. (It also serves as a rest.)
      sound_eg_key_off(&ctx.stch->eg);
      if (ctx.stch->hw_envelope_enable) {
        PSG_SET(8+ctx.ch, 0);
      }
      break;
    case 0x2:
    case 0x3:
      // noise (5bits)
      // 001b fdr:5
      // -- Changes frequency of noise generator.
      PSG_SET(6, (x & 0x1f));
      break;
    case 0x7:
      // mixer (6bits)
      // 0111b ----b --b NC:1 NB:1 NA:1 TC:1 TB:1 TA:1
      // -- Changes the mixer setting.
      {
        len--;
        // const uint8_t mixer = (*next++ & 0x3f) | 0x80;
        const uint8_t mixer = *next++;
        PSG_SET(7, mixer);
      }
      break;
    case 0x8:
      // volume (4bits)
      // 1000b vol:4
      // -- Changes the amplifier level.
      {
        ctx.stch->amp = x & 0x0f;
        ctx.stch->hw_envelope_enable = false;
      }
      break;
    case 0xc:
      // software envelope pattern (4bits)
      // 1100b pat:4
      // -- Set the software envelope pattern.
      {
        sound_channel_set_envelope(ctx.stch, x & 0x0f);
        ctx.stch->hw_envelope_enable = false;
      }
      break;
    case 0x4:
    case 0x9:
    case 0xd:
      if (x & 0x90) {
        // envelope pattern (4bits)
        // 1-01b pat:4
        // -- Set the hardware envelope pattern, and enable hardware envelope.
        // The envelope pattern and switch states are saved (not applied to
        // the registers here) and applied at each "key on" timing.
        ctx.st->envelope_pattern = x & 0x0f;
        ctx.stch->hw_envelope_enable = true;
      }
      if (x & 0x40) {
        // envelope cycle (16bits)
        // -10-b ----b fdr_hi:8 fdr_lo:8
        // -- Set the hardware envelope cycle.
        const uint8_t cycle_hi = *next++;
        const uint8_t cycle_lo = *next++;
        len -= 2;
        PSG_SET(12, cycle_hi); // cycle_hi:8
        PSG_SET(11, cycle_lo); // cycle_lo:8
      }
      break;
    default:
      // stream error
      len = 0;
      ctx.st->flag &= ~ctx.mask;
      break;
    }
  }
}

static bool sound_player__process(void) {
  while (ctx.st->section < ctx.st->clip->num_fragments) {
    {
      const uint8_t count = ctx.st->count_per_tick * sound_advance_count;
      ctx.mask = SOUND_CHANNEL_A;
      for (ctx.ch = 0; ctx.ch < 3; ++ctx.ch, ctx.mask <<= 1) {
        if (ctx.st->flag & ctx.mask) {
          ctx.channel_muted = (bool)(ctx.mute & ctx.mask);
          ctx.stch = &ctx.st->channels[ctx.ch];
          // ---- Countdown time remaining ----
          if (count < ctx.stch->duration) {
            ctx.stch->duration -= count;
          } else {
            ctx.stch->duration = 0;
            // ---- Decode next chunk ----
            sound_player__process_channel();
          }
          if (ctx.channel_muted || ctx.stch->hw_envelope_enable) {
            continue;
          }
          sound_channel_output();
        }
      }
    }
    if (ctx.st->flag & SOUND_CHANNEL_ALL) {
      // current fragment is not finished.
      return false;
    }
    if (++ctx.st->section >= ctx.st->clip->num_fragments) {
      // end of the clip
      break;
    }
    // the clip has more fragments
    const struct sound_fragment * sf = ctx.st->clip->fragments[ctx.st->section];
    if (!sf) {
      break;
    }
    // set the next fragment
    sound_set_fragment(ctx.st, sf);
    if (!(ctx.st->flag & SOUND_CHANNEL_ALL)) {
      // end of the clip (invalid fragment)
      break;
    }
    // start to play the next fragment.
  }
  ctx.st->section = ctx.st->clip->num_fragments;
  return true;
}

// -----
void sound_player(void) {
  if (sound.paused) {
    return;
  }
  ay_3_8910_play();
  sound_effect_apply();
  sound_update_tick();
  // ---- background music ----
  if (sound.bg.clip) {
    // By passing `mute` as 2nd parameter, temporarily turn on the mute switch
    // for the channel being used for sound effects.
    ctx.st = &sound.bg;
    ctx.mute = ((sound.bg.flag >> 3) | sound.se.flag) & 7;
    if (sound_player__process()) {
      // ---- end of music ----
      if (sound.repeat) {
        // ---- auto-repeat ----
        sound_set_bgm0(sound.bg.clip);
        sound_start0();
      }
      else {
        sound_state_init(&sound.bg);
      }
    }
  }
  // ---- sound effect ----
  if (sound.se.clip) {
    ctx.st = &sound.se;
    ctx.mute = (sound.se.flag >> 3) & 7;
    if (sound_player__process()) {
      // ---- end of music ----
      sound_state_init(&sound.se);
      sound_restore_psg_registers();
    }
  }
}

void sound_set_eg_table(const struct sound_eg_attribute * table) {
  DI();
  sound_set_eg_table0(table);
  EI();
}

void sound_set_speed(uint8_t multiplier) {
  DI();
  sound_set_speed0(multiplier);
  EI();
}

void sound_set_repeat(bool repeat) {
  DI();
  sound_set_repeat0(repeat);
  EI();
}

void sound_set_volume(uint8_t volume) {
  DI();
  sound_set_volume0(volume);
  EI();
}

void sound_set_mute(uint8_t mute) {
  DI();
  sound_set_mute0(mute);
  EI();
}

void sound_effect(const struct sound_clip* s) {
  DI();
  sound_effect0(s);
  EI();
}

void sound_set_bgm(const struct sound_clip* s) {
  DI();
  sound_set_bgm0(s);
  EI();
}

void sound_start(void) {
  DI();
  sound_start0();
  EI();
}

void sound_init(void) {
  DI();
  sound_init0();
  EI();
}

void sound_stop(void) {
  DI();
  sound_stop0();
  EI();
}

void sound_pause(void) {
  DI();
  sound_pause0();
  EI();
}
