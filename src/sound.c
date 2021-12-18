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

#include <string.h>

#include "../include/bios.h"
#include "../include/psg.h"
#include "../include/sound.h"
#include "../include/sound_eg.h"
#include "../include/sound_eg_spi.h"

#define COUNT_PER_SECOND    (1200)
#define COUNT_PER_TICK_60HZ (COUNT_PER_SECOND / 60)

static uint8_t COUNT_PER_TICK;
static uint8_t VSYNC_FREQ;

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

void sound_set_eg_table(const struct sound_eg_attribute * table) {
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

void sound_set_speed(uint8_t multiplier) {
  if (8 < multiplier) {
    multiplier = 8;
  }
  COUNT_PER_TICK = COUNT_PER_SECOND * multiplier / VSYNC_FREQ / 4;
}

void sound_set_repeat(bool repeat) {
  sound.repeat = repeat;
}

static struct {
  uint8_t noise_fdr;
  uint8_t mixer;
  uint8_t envelope_cycle_lo;
  uint8_t envelope_cycle_hi;
} backup;

static void sound_backup_psg_registers(void) {
  backup.noise_fdr = psg_get(6);
  backup.mixer = psg_get(7);
  backup.envelope_cycle_lo = psg_get(11);
  backup.envelope_cycle_hi = psg_get(12);
}

static void sound_restore_psg_registers(void) {
  sound.bg.channels[0].eg.output = 0; psg_set(8, 0);
  sound.bg.channels[1].eg.output = 0; psg_set(9, 0);
  sound.bg.channels[2].eg.output = 0; psg_set(10, 0);
  psg_set(6, backup.noise_fdr);
  psg_set(7, backup.mixer);
  psg_set(11, backup.envelope_cycle_lo);
  psg_set(12, backup.envelope_cycle_hi);
}

void sound_set_volume(uint8_t volume) {
  sound.volume = (15 < volume ? 15 : volume);
}

void sound_set_mute(uint8_t mute) {
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

static void sound_set_clip(struct sound_state* st, const struct sound_clip* s) {
  if (!s || !s->fragments || 0 == s->num_fragments) {
    st->flag = 0;
    st->clip = 0;
    return;
  }
  st->section = 0;
  for (uint8_t ch = 0; ch < 3; ++ch) {
    sound_channel_reset(&st->channels[ch]);
  }
  const struct sound_fragment* sf = s->fragments[0];
  sound_set_fragment(st, sf);
  st->clip = s;
}

static volatile const struct sound_clip * next_sound_effect;
void sound_effect(const struct sound_clip* s) {
  __asm__("di");
  next_sound_effect = s;
  __asm__("ei");
}

static void sound_effect_apply(void) {
  const struct sound_clip * s = next_sound_effect;
  next_sound_effect = NULL;
  if (!s) return;
  if (sound.se.flag & SOUND_CHANNEL_ALL) {
    if (s->priority < sound.se.clip->priority) {
      return;
    }
  }
  if (sound.se.clip) {
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

void sound_set_bgm(const struct sound_clip* s) {
  sound_stop();
  sound_pause();
  if (!s) {
    return;
  }
  __critical {
    sound_set_clip(&sound.bg, s);
  }
}

void sound_start(void) {
  sound.paused = false;
}

static void sound_state_init(struct sound_state * st) {
  st->clip = 0;
  st->flag = 0;
  // ----
  for (uint8_t ch = 0; ch < 3; ++ch) {
    sound_channel_reset(&st->channels[ch]);
  }
  st->envelope_pattern = psg_reg_initial_vector[13];
}

void sound_init(void) {
  VSYNC_FREQ = msx_get_vsync_frequency();
  sound_set_speed(SOUND_SPEED_1X);
  sound_eg_table = sound_eg_table_default;
  sound_set_volume(15);
  sound_stop();
}

void sound_stop(void) {
  sound_effect(NULL);
  sound_pause();
  psg_init();
  sound_state_init(&sound.bg);
  sound_state_init(&sound.se);
  sound_tick = 0;
}

void sound_pause(void) {
  sound.paused = true;
  psg_set(8, 0);
  psg_set(9, 0);
  psg_set(10, 0);
}

// ---- local work area for `sound_player()` ----
static struct sound_state* st;
static uint8_t mute;

static void sound_channel_output(const uint8_t ch,
                                 const struct sound_channel * stch) {
  // if using hardware envelope, it shall be treated specially.
  // otherwise, the amplitude is controlled by software envelope.
  const uint8_t x = ((uint16_t)stch->eg.output *
                     (stch->amp + 1) *
                     (sound.volume + 1)) >> 12;
  psg_set(8+ch, x);
}

inline void sound_player__process_channel(const uint8_t ch, const uint8_t mask) {
  const bool channel_muted = (bool)(mute & (mask << 3));
  struct sound_channel * const stch = &st->channels[ch];
  // advance the channel's envelope generator by 1/60th a second.
  sound_eg_advance(&stch->eg);
  // ---- Countdown time remaining ----
  {
    stch->duration -= COUNT_PER_TICK;
    if (0 < stch->duration) {
      if (!channel_muted && !stch->hw_envelope_enable) {
        sound_channel_output(ch, stch);
      }
      return;
    }
  }
  // ---- Reads head (chunk size and next duration) ----
  uint8_t len;
  {
    const uint8_t head1 = *stch->next++;
    if (head1 == 255) {
      // end of music track
      st->flag &= ~mask;
      if (!channel_muted && !stch->hw_envelope_enable) {
        sound_channel_output(ch, stch);
      }
      return;
    }
    const uint16_t ticks = ((head1 & 0x1f) << 8) | (*stch->next++);
    stch->duration += COUNT_PER_TICK_60HZ * ticks;
    len = (head1 >> 5) & 7;
  }
  // ---- Parse and process the chunk ----
  while (len--) {
    const uint8_t x = *stch->next++;
    const uint8_t x_lo = x & 0x0f;
    switch (x & 0xf0) {
    case 0xe0:
      // tone (12bits)
      // 1110b fdr_hi:4 fdr_lo:8
      // -- changes frequency of tone generator but do not key-on.
      {
        const uint8_t x_hi = *stch->next++;
        len--;
        if (!channel_muted) {
          psg_set(1+2*ch, x_lo);    // fdr_hi:4
          psg_set(0+2*ch, x_hi);    // fdr_lo:8
        }
      }
      break;
    case 0x00:
      // keyon (12bits)
      // 0000b fdr_hi:4 fdr_lo:8
      // -- Changes frequency of tone generator and performs a key-on.
      {
        const uint8_t x_hi = *stch->next++;
        len--;
        if (!channel_muted) {
          psg_set(1+2*ch, x_lo);    // fdr_hi:4
          psg_set(0+2*ch, x_hi);    // fdr_lo:8
          // When using hardware envelopes, special processing is required.
          if (stch->hw_envelope_enable) {
            psg_set(13, st->envelope_pattern);
            psg_set(8+ch, 16);
          }
        }
        sound_eg_key_on(&stch->eg);
      }
      break;
    case 0x10:
      // keyoff (0bits)
      // -- Perform a key-off. (It also serves as a rest.)
      sound_eg_key_off(&stch->eg);
      if (stch->hw_envelope_enable && !channel_muted) {
        psg_set(8+ch, 0);
      }
      break;
    case 0x20:
    case 0x30:
      // noise (5bits)
      // 001b fdr:5
      // -- Changes frequency of noise generator.
      if (!channel_muted) {
        psg_set(6, (x & 0x1f));
      }
      break;
    case 0x70:
      // mixer (6bits)
      // 0111b ----b --b NC:1 NB:1 NA:1 TC:1 TB:1 TA:1
      // -- Changes the mixer setting.
      {
        const uint8_t mixer = (*stch->next++ & 0x3f) | 0x80;
        len--;
        if (!channel_muted) {
          psg_set(7, mixer);
        }
      }
      break;
    case 0x80:
      // volume (4bits)
      // 1000b vol:4
      // -- Changes the amplifier level.
      stch->amp = x_lo;
      stch->hw_envelope_enable = false;
      break;
    case 0xc0:
      // software envelope pattern (4bits)
      // 1100b pat:4
      // -- Set the software envelope pattern.
      sound_channel_set_envelope(stch, x_lo);
      stch->hw_envelope_enable = false;
      break;
    case 0x40:
    case 0x90:
    case 0xd0:
      if (x & 0x90) {
        // envelope pattern (4bits)
        // 1-01b pat:4
        // -- Set the hardware envelope pattern, and enable hardware envelope.
        // The envelope pattern and switch states are saved (not applied to
        // the registers here) and applied at each "key on" timing.
        st->envelope_pattern = x_lo;
        stch->hw_envelope_enable = true;
      }
      if (x & 0x40) {
        // envelope cycle (16bits)
        // -10-b ----b fdr_hi:8 fdr_lo:8
        // -- Set the hardware envelope cycle.
        const uint8_t cycle_hi = *stch->next++;
        const uint8_t cycle_lo = *stch->next++;
        len -= 2;
        if (!channel_muted) {
          psg_set(12, cycle_hi); // cycle_hi:8
          psg_set(11, cycle_lo); // cycle_lo:8
        }
      }
      break;
    default:
      // stream error
      len = 0;
      st->flag &= ~mask;
      break;
    }
  }
  if (!channel_muted && !stch->hw_envelope_enable) {
    sound_channel_output(ch, stch);
  }
}

static bool sound_player__process(void) {
  for (;;) {
    {
      uint8_t mask = SOUND_CHANNEL_A;
      for (uint8_t ch = 0; ch < 3; ++ch, mask <<= 1) {
        if (st->flag & mask) {
          sound_player__process_channel(ch, mask);
        }
      }
    }
    if (st->flag & SOUND_CHANNEL_ALL) {
      // current fragment is not finished.
      return false;
    }
    if (++st->section >= st->clip->num_fragments) {
      // end of the clip
      return true;
    }
    // the clip has more fragments
    const struct sound_fragment * sf = st->clip->fragments[st->section];
    if (sf) {
      // set the next fragment
      sound_set_fragment(st, sf);
    }
    if (!(st->flag & SOUND_CHANNEL_ALL)) {
      // end of the clip (invalid fragment)
      return true;
    }
    // start to play the next fragment.
  }
}

void sound_player(void) {
  if (sound.paused) {
    return;
  }
  sound_effect_apply();
  sound_update_tick();
  // ---- sound effect ----
  if (sound.se.clip) {
    st = &sound.se;
    // mute = sound.se.flag & (7 << 3);
    mute = sound.se.flag;
    if (sound_player__process()) {
      // ---- end of music ----
      sound.se.clip = 0;
      sound_restore_psg_registers();
    }
  }
  // ---- background music ----
  if (sound.bg.clip) {
    // By passing `mute` as 2nd argument, temporarily turn on the mute switch
    // for the channel being used for sound effects.
    st = &sound.bg;
    // mute = (sound.bg.flag & (7 << 3)) | ((sound.se.flag & 7) << 3);
    mute = sound.bg.flag | (sound.se.flag << 3);
    if (sound_player__process()) {
      // ---- end of music ----
      if (sound.repeat) {
        // ---- auto-repeat ----
        sound_set_bgm(sound.bg.clip);
        sound_start();
      }
    }
  }
}
