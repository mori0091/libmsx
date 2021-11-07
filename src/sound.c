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

#include "../include/psg.h"
#include "../include/sound.h"

static const __at (0x002b) uint8_t INTERNATIONAL_ID_1;
static const __at (0x002c) uint8_t INTERNATIONAL_ID_2;

#define COUNT_PER_SECOND    (300)
#define COUNT_PER_TICK_60HZ (COUNT_PER_SECOND / 60)

static uint16_t COUNT_PER_TICK;
static uint8_t VSYNC_FREQ;

struct sound_state {
  const struct sound_clip* clip; // pointer to current music (set of streams)
  const uint8_t* next[3];        // pointer to next chunk for each streams
  int16_t duration[3];           // remaining duration for each channel
  uint8_t flag;                  // playing/stopped flags
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

static uint8_t psg_reg_backup[16];

static void sound_psg_set(bool muted, uint8_t reg, uint8_t value) {
  if (!muted) {
    psg_set(reg, value);
  }
  else {
    psg_reg_backup[reg] = value;
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

void sound_effect(const struct sound_clip* s) {
  if (!s) return;
  if (sound.se.state.flag & SOUND_CHANNEL_ALL) {
    if (s->priority < sound.se.state.clip->priority) {
      return;
    }
  }
  if (!sound.se.state.clip) {
    // backup some PSG registers
    for (uint8_t i = 0; i <= 13; ++i) {
      psg_reg_backup[i] = psg_get(i);
    }
  }
  sound.se.state.next[0] = s->streams[0];
  sound.se.state.next[1] = s->streams[1];
  sound.se.state.next[2] = s->streams[2];
  sound.se.state.duration[0] = 0;
  sound.se.state.duration[1] = 0;
  sound.se.state.duration[2] = 0;
  __critical {
    sound.se.state.flag = SOUND_CHANNEL_ALL;
    sound.se.state.clip = s;
  }
}

void sound_set_bgm(const struct sound_clip* s) {
  sound_stop();
  sound_pause();
  if (s) {
    sound.bg.state.next[0] = s->streams[0];
    sound.bg.state.next[1] = s->streams[1];
    sound.bg.state.next[2] = s->streams[2];
    sound.bg.state.duration[0] = 0;
    sound.bg.state.duration[1] = 0;
    sound.bg.state.duration[2] = 0;
    sound.bg.state.flag = SOUND_CHANNEL_ALL;
    sound.bg.state.clip = s;
  }
}

void sound_start(void) {
  sound.bg.paused = false;
}

static void sound_init(void) {
  VSYNC_FREQ = ((INTERNATIONAL_ID_1 & 0x80) ? 50 : 60);
  COUNT_PER_TICK = COUNT_PER_SECOND / VSYNC_FREQ;
  psg_init();
  sound.bg.state.clip = 0;
  sound.bg.state.flag = 0;
  sound.se.state.clip = 0;
  sound.se.state.flag = 0;
  memset(psg_reg_backup, 0, sizeof(psg_reg_backup));
  psg_set(7, 0x80 | 0x38);      // mixer
}

void sound_stop(void) {
  sound_pause();
  sound_init();
}

void sound_pause(void) {
  sound.bg.paused = true;
  psg_set(8, 0);
  psg_set(9, 0);
  psg_set(10, 0);
}

static void sound_player__process(struct sound_state* st, uint8_t flag) {
  uint8_t mask = SOUND_CHANNEL_A;
  for (uint8_t ch = 0; ch < 3; ++ch, mask <<= 1) {
    // ---- Countdown time remaining ----
    if (0 < st->duration[ch]) {
      st->duration[ch] -= COUNT_PER_TICK;
      if (0 < st->duration[ch]) {
        continue;
      }
    }
    // ---- Check to see if the stream has finished. ----
    if (!(st->flag & mask)) continue; // no more chunk
    uint8_t head1 = *st->next[ch]++;
    if (head1 == 255) {
      // end of music track
      st->flag &= ~mask;
      continue;
    }
    // ---- Reads head (chunk size and next duration) ----
    uint8_t len = (head1 >> 5) & 7;
    if (!len) len = 8;          // 'length = 0' means 8 bytes chunk
    const uint16_t ticks = ((head1 & 0x1f) << 8) | (*st->next[ch]++);
    st->duration[ch] += COUNT_PER_TICK_60HZ * ticks;
    // ---- Parse and process the chunk ----
    bool muted = flag & (mask << 3); // \note Using `flag` instead of `st->flag`
    while (len) {
      len--;
      uint8_t x = *st->next[ch]++;
      uint8_t x_lo = x & 0x0f;
      switch (x & 0xf0) {
      case 0x00:
        // tone (12bits)
        // 0000b fdr_hi:4 fdr_lo:8
        sound_psg_set(muted, 1+2*ch, x_lo);            // fdr_hi:4
        sound_psg_set(muted, 0+2*ch, *st->next[ch]++); // fdr_lo:8
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
        sound_psg_set(muted, 7, (*st->next[ch]++ & 0x3f) | 0x80);
        break;
      case 0x80:
        // volume (4bits)
        // 1000b vol:4
        sound_psg_set(muted, 8+ch, x_lo);
        break;
      case 0x40:
      case 0x90:
      case 0xd0:
        if (x & 0x90) {
          // envelope pattern (4bits)
          // 1-01b pat:4
          sound_psg_set(muted, 13, x_lo);
          sound_psg_set(muted, 8+ch, 16); // turn on hardware envelope
        }
        if (x & 0x40) {
          // envelope cycle (16bits)
          // -10-b ----b fdr_hi:8 fdr_lo:8
          sound_psg_set(muted, 12, *st->next[ch]++); // cyble_hi:8
          sound_psg_set(muted, 11, *st->next[ch]++); // cycle_lo:8
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

void sound_player(void) {
  // ---- auto-repeat ----
  if (!(sound.bg.state.flag & SOUND_CHANNEL_ALL)) {
    if (sound.bg.repeat) {
      sound_set_bgm(sound.bg.state.clip);
      sound_start();
    }
  }
  // ---- background music ----
  if (!sound.bg.paused && sound.bg.state.clip) {
    // By passing `flag` as 2nd argument, temporarily turn on the mute switch
    // for the channel being used for sound effects.
    sound_player__process(&sound.bg.state, ((sound.bg.state.flag) |
                                            (sound.se.state.flag << 3)));
  }
  // ---- sound effect ----
  if (sound.se.state.clip) {
    sound_player__process(&sound.se.state, sound.se.state.flag);
    uint8_t flag = sound.se.state.flag & SOUND_CHANNEL_ALL;
    if (!flag) {
      sound.se.state.clip = 0;
      // restore some PSG registers
      for (uint8_t i = 0; i <= 13; ++i) {
        psg_set(i, psg_reg_backup[i]);
      }
    }
  }
}