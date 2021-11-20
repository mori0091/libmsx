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

// backup of PSG registers
static uint8_t sound_psg_reg_backup[14];

#define COUNT_PER_SECOND    (300)
#define COUNT_PER_TICK_60HZ (COUNT_PER_SECOND / 60)

static uint16_t COUNT_PER_TICK;
static uint8_t VSYNC_FREQ;

struct channel {
  const uint8_t* next;          // pointer to next chunk for each streams
  int16_t duration;             // remaining duration for each channel
  uint8_t volume;               // current volume for each channel
};

struct sound_state {
  const struct sound_clip* clip; // pointer to current music (set of streams)
  struct channel channels[3];
  uint8_t section;               // current fragment number
  uint8_t flag;                  // playing/stopped flags
  uint8_t envelope_pattern;      // current envelope pattern
};

static struct {
  // background music
  struct {
    struct sound_state state;   // state of background music
    bool repeat;                // auto repeat on/off
    bool paused;                // whether the BGM is paused or not.
  } bg;
  // sound effect
  struct {
    struct sound_state state;   // state of sound effect
  } se;
} sound;

void sound_set_repeat(bool repeat) {
  sound.bg.repeat = repeat;
}

static void sound_psg_set(bool muted, uint8_t reg, uint8_t value) {
  if (!muted) {
    psg_set(reg, value);
  }
  else {
    sound_psg_reg_backup[reg] = value;
  }
}

static void sound_backup_psg_registers(void) {
  for (uint8_t i = 0; i <= 13; ++i) {
    sound_psg_reg_backup[i] = psg_get(i);
  }
}

static void sound_restore_psg_registers(void) {
  // tone, noise, and mixier
  for (uint8_t i = 0; i <= 7; ++i) {
    psg_set(i, sound_psg_reg_backup[i]);
  }
  // volume and/or envelope on/off
  for (uint8_t i = 8; i <= 10; ++i) {
    if (sound_psg_reg_backup[i] & 16) {
      psg_set(i, 0);
    } else {
      psg_set(i, sound_psg_reg_backup[i]);
    }
  }
  // envelope pattern and cycle
  for (uint8_t i = 11; i <= 13; ++i) {
    psg_set(i, sound_psg_reg_backup[i]);
  }
}

void sound_set_mute(uint8_t mute) {
  mute &= 7;
  sound.bg.state.flag &= ~(7 << 3);
  sound.bg.state.flag |= mute << 3;
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
  __critical {
    st->flag = flag;
  }
}

static void sound_set_clip(struct sound_state* st, const struct sound_clip* s) {
  if (!s || !s->fragments || 0 == s->num_fragments) {
    __critical {
      st->flag = 0;
      st->clip = 0;
    }
    return;
  }
  st->section = 0;
  st->channels[0].volume = psg_reg_initial_vector[8];
  st->channels[1].volume = psg_reg_initial_vector[9];
  st->channels[2].volume = psg_reg_initial_vector[10];
  const struct sound_fragment* sf = s->fragments[0];
  sound_set_fragment(st, sf);
  __critical {
    st->clip = s;
  }
}

static const struct sound_clip * next_sound_effect;
void sound_effect(const struct sound_clip* s) {
  next_sound_effect = s;
}

static void sound_effect_apply(void) {
  const struct sound_clip * s = next_sound_effect;
  next_sound_effect = NULL;
  if (!s) return;
  if (sound.se.state.flag & SOUND_CHANNEL_ALL) {
    if (s->priority < sound.se.state.clip->priority) {
      return;
    }
  }
  if (!sound.se.state.clip) {
    // BGM may be playing, so takes snapshot of register values.
    sound_backup_psg_registers();
  }
  else {
    // When switching sound effects, using channels may differ between previous
    // and next sound effects, so temporally restores register values for BGM
    // before playing the next sound effects.
    sound_restore_psg_registers();
  }
  sound_set_clip(&sound.se.state, s);
}

void sound_set_bgm(const struct sound_clip* s) {
  sound_stop();
  sound_pause();
  if (s) {
    sound_set_clip(&sound.bg.state, s);
  }
}

void sound_start(void) {
  sound.bg.paused = false;
}

void sound_init(void) {
  sound_effect(NULL);
  sound_pause();
  VSYNC_FREQ = msx_get_vsync_frequency();
  COUNT_PER_TICK = COUNT_PER_SECOND / VSYNC_FREQ;
  psg_init();
  sound.bg.state.clip = 0;
  sound.bg.state.flag = 0;
  sound.se.state.clip = 0;
  sound.se.state.flag = 0;
  memcpy(sound_psg_reg_backup,
         psg_reg_initial_vector,
         sizeof(psg_reg_initial_vector));
}

void sound_stop(void) {
  sound_init();
}

void sound_pause(void) {
  sound.bg.paused = true;
  psg_set(8, 0);
  psg_set(9, 0);
  psg_set(10, 0);
}

static void sound_player__process_chunk(struct sound_state* st, const uint8_t flag) {
  uint8_t mask = SOUND_CHANNEL_A;
  for (uint8_t ch = 0; ch < 3; ++ch, mask <<= 1) {
    struct channel * stch = &st->channels[ch];
    // ---- Countdown time remaining ----
    if (0 < stch->duration) {
      stch->duration -= COUNT_PER_TICK;
      if (0 < stch->duration) {
        continue;
      }
    }
    // ---- Check to see if the stream has finished. ----
    if (!(st->flag & mask)) continue; // no more chunk
    const uint8_t head1 = *stch->next++;
    if (head1 == 255) {
      // end of music track
      st->flag &= ~mask;
      continue;
    }
    // ---- Reads head (chunk size and next duration) ----
    uint8_t len = (head1 >> 5) & 7;
    const uint16_t ticks = ((head1 & 0x1f) << 8) | (*stch->next++);
    stch->duration += COUNT_PER_TICK_60HZ * ticks;
    // ---- Parse and process the chunk ----
    const bool muted = flag & (mask << 3); // \note Using `flag` instead of `st->flag`
    while (len) {
      len--;
      const uint8_t x = *stch->next++;
      const uint8_t x_lo = x & 0x0f;
      switch (x & 0xf0) {
      case 0x00:
        // tone (12bits)
        // 0000b fdr_hi:4 fdr_lo:8
        sound_psg_set(muted, 1+2*ch, x_lo);          // fdr_hi:4
        sound_psg_set(muted, 0+2*ch, *stch->next++); // fdr_lo:8
        // "note on"
        {
          if (16 & stch->volume) {
            // (re)set hardware envelope
            sound_psg_set(muted, 13, st->envelope_pattern);
            sound_psg_set(muted, 8+ch, stch->volume);
          }
          else {
            // (re)set software envelope
            // \TODO implement software envelope functionality
          }
        }
        len--;
        break;
      case 0x10:
        // rest (0bits)
        sound_psg_set(muted, 8+ch, 0);
        break;
      case 0x20:
      case 0x30:
        // noise (5bits)
        // 001b fdr:5
        sound_psg_set(muted, 6, x & 0x1f);
        break;
      case 0x70:
        // mixer (6bits)
        // 0111b ----b --b NC:1 NB:1 NA:1 TC:1 TB:1 TA:1
        sound_psg_set(muted, 7, (*stch->next++ & 0x3f) | 0x80);
        break;
      case 0x80:
        // volume (4bits)
        // 1000b vol:4
        sound_psg_set(muted, 8+ch, x_lo);
        stch->volume = x_lo;
        break;
      case 0x40:
      case 0x90:
      case 0xd0:
        if (x & 0x90) {
          // envelope pattern (4bits)
          // 1-01b pat:4
          // \note
          // The envelope pattern and switch states are saved (not applied to
          // the registers here) and applied at each "note on" timing.
          st->envelope_pattern = x_lo; // envelope pattern
          stch->volume |= 16;     // turn on hardware envelope
        }
        if (x & 0x40) {
          // envelope cycle (16bits)
          // -10-b ----b fdr_hi:8 fdr_lo:8
          sound_psg_set(muted, 12, *stch->next++); // cyble_hi:8
          sound_psg_set(muted, 11, *stch->next++); // cycle_lo:8
          len -= 2;
        }
        break;
      default:
        // stream error
        len = 0;
        st->flag &= ~mask;
        break;
      }
    }
  }
}

static bool sound_player__process(struct sound_state* st, uint8_t mute) {
  for (;;) {
    sound_player__process_chunk(st, st->flag | mute);
    if (st->flag & SOUND_CHANNEL_ALL) {
      // current fragment is not finished.
      return false;
    }
    if (st->section + 1 >= st->clip->num_fragments) {
      // end of the clip
      return true;
    }
    // the clip has more fragments
    st->section++;
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
  sound_effect_apply();
  // ---- sound effect ----
  if (sound.se.state.clip) {
    bool finished = sound_player__process(&sound.se.state, 0);
    if (finished) {
      sound.se.state.clip = 0;
      sound_restore_psg_registers();
    }
  }
  // ---- background music ----
  if (!sound.bg.paused && sound.bg.state.clip) {
    // By passing `mute` as 2nd argument, temporarily turn on the mute switch
    // for the channel being used for sound effects.
    bool finished = sound_player__process(&sound.bg.state,
                                          (sound.se.state.flag << 3));
    // ---- auto-repeat ----
    if (finished) {
      if (sound.bg.repeat) {
        sound_set_bgm(sound.bg.state.clip);
        sound_start();
      }
    }
  }
}
