// -*- coding: utf-8-unix -*-

#include <stdlib.h>
#include <msx.h>

#include "sound.h"

static const __at (0x002b) uint8_t INTERNATIONAL_ID_1;
static const __at (0x002c) uint8_t INTERNATIONAL_ID_2;

#define COUNT_PER_SECOND    (300)
#define COUNT_PER_TICK_60HZ (COUNT_PER_SECOND / 60)

static uint16_t COUNT_PER_TICK;
static uint8_t VSYNC_FREQ;

static struct {
  struct sound_clip* data;      // pointer to current music (set of streams)
  uint8_t* next[3];             // pointer to next chunk for each streams
  int16_t duration[3];          // remaining duration for each channel
  uint8_t flag;                 // playing/stopped flags
  bool repeat;                  // auto repeat on/off
} sound;

void sound_set_repeat(bool repeat) {
  sound.repeat = repeat;
}

void sound_set_mute(uint8_t mute) {
  // \TODO implement
}

void sound_start(struct sound_clip* s) {
  sound_stop();
  if (s) {
    sound.data = s;
    sound.next[0] = s->streams[0];
    sound.next[1] = s->streams[1];
    sound.next[2] = s->streams[2];
    sound.duration[0] = 0;
    sound.duration[1] = 0;
    sound.duration[2] = 0;
    sound.flag = SOUND_CHANNEL_ALL;
  }
}

void sound_stop(void) {
  VSYNC_FREQ = ((INTERNATIONAL_ID_1 & 0x80) ? 50 : 60);
  COUNT_PER_TICK = COUNT_PER_SECOND / VSYNC_FREQ;
  psg_init();
  sound.data = 0;
  sound.flag = 0;
}

void sound_pause(void) {
  // \TODO implement
}

void sound_player(void) {
  if (!sound.data) return;
  if (!(sound.flag & SOUND_CHANNEL_ALL)) {
    if (!sound.repeat) {
      sound_stop();
      return;
    }
    sound_start(sound.data);
  }
  uint8_t mask = SOUND_CHANNEL_A;
  for (uint8_t ch = 0; ch < 3; ++ch, mask <<= 1) {
    // ---- Countdown time remaining ----
    if (0 < sound.duration[ch]) {
      sound.duration[ch] -= COUNT_PER_TICK;
      if (0 < sound.duration[ch]) {
        continue;
      }
    }
    // ---- Check to see if the stream has finished. ----
    if (!(sound.flag & mask)) continue; // no more chunk
    uint8_t head1 = *sound.next[ch]++;
    if (head1 == 255) {
      // end of music track
      sound.flag &= ~mask;
      continue;
    }
    // ---- Reads head (chunk size and next duration) ----
    uint8_t len = (head1 >> 5) & 7;
    if (!len) len = 8;          // 'length = 0' means 8 bytes chunk
    const uint16_t ticks = ((head1 & 0x1f) << 8) | (*sound.next[ch]++);
    sound.duration[ch] += COUNT_PER_TICK_60HZ * ticks;
    // ---- Parse and process the chunk ----
    while (len) {
      len--;
      uint8_t x = *sound.next[ch]++;
      uint8_t x_lo = x & 0x0f;
      switch (x & 0xf0) {
      case 0x00:
        // tone (12bits)
        // 0000b fdr_hi:4 fdr_lo:8
        psg_set(1+2*ch, x_lo);              // fdr_hi:4
        psg_set(0+2*ch, *sound.next[ch]++); // fdr_lo:8
        len--;
        break;
      case 0x20:
      case 0x30:
        // noise (5bits)
        // 001b fdr:5
        psg_set(6, x & 0x1f);
        break;
      case 0x70:
        // mixer (6bits)
        // 0111b ----b --b NC:1 NB:1 NA:1 TC:1 TB:1 TA:1
        psg_set(7, (*sound.next[ch]++ & 0x3f) | 0x80);
        break;
      case 0x80:
        // volume (4bits)
        // 1000b vol:4
        psg_set(8+ch, x_lo);
        break;
      case 0x40:
      case 0x90:
      case 0xd0:
        if (x & 0x90) {
          // envelope pattern (4bits)
          // 1-01b pat:4
          psg_set(13, x_lo);
          psg_set(8+ch, 16);  // turn on hardware envelope
        }
        if (x & 0x40) {
          // envelope cycle (16bits)
          // -10-b ----b fdr_hi:8 fdr_lo:8
          psg_set(12, *sound.next[ch]++); // cyble_hi:8
          psg_set(11, *sound.next[ch]++); // cycle_lo:8
          len -= 2;
        }
        break;
      default:
        // stream error
        len = 0;
        sound.flag &= ~mask;
        break;
      }
    }
  }
}
