// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2022 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file snd_osc_period.c
 *
 */

#include "./snd_osc_period.h"

static const uint16_t osc_periods[OSC_PERIODS_INDEX_MAX] = {
  // C      C#     D      D#     E      F      F#     G      G#     A      A#     B
                                                                 0xfe4, 0xf00, 0xe28, // octave 0
  0xd5d, 0xc9c, 0xbe7, 0xb3c, 0xa9b, 0xa02, 0x973, 0x8eb, 0x86b, 0x7f2, 0x780, 0x714, // octave 1 ( #24..)
  0x6af, 0x64e, 0x5f4, 0x59e, 0x54e, 0x501, 0x4ba, 0x476, 0x436, 0x3f9, 0x3c0, 0x38a, // octave 2 ( #36..)
  0x357, 0x327, 0x2fa, 0x2cf, 0x2a7, 0x281, 0x25d, 0x23b, 0x21b, 0x1fd, 0x1e0, 0x1c5, // octave 3 ( #48..)
  0x1ac, 0x194, 0x17d, 0x168, 0x153, 0x140, 0x12e, 0x11d, 0x10d, 0x0fe, 0x0f0, 0x0e3, // octave 4 ( #60..)
  0x0d6, 0x0ca, 0x0be, 0x0b4, 0x0aa, 0x0a0, 0x097, 0x08f, 0x087, 0x07f, 0x078, 0x071, // octave 5 ( #72..)
  0x06b, 0x065, 0x05f, 0x05a, 0x055, 0x050, 0x04c, 0x047, 0x043, 0x040, 0x03c, 0x039, // octave 6 ( #84..)
  0x035, 0x032, 0x030, 0x02d, 0x02a, 0x028, 0x026, 0x024, 0x022, 0x020, 0x01e, 0x01c, // octave 7 ( #96..)
  0x01b, 0x019, 0x018, 0x016, 0x015, 0x014, 0x013, 0x012, 0x011, 0x010, 0x00f, 0x00e, // octave 8 (#108..)
};

/**
 * Multiply an 8 bit coefficient `k` to `x`.
 *
 * \param k  an 8 bit coefficient (0..255 means 0..255/256)
 * \param x  a 16 bit unsigned integer
 * \return   the integer part of k*x
 */
static uint16_t kxQ8(uint8_t k, uint16_t x) {
  uint16_t h = (k * (x >> 8));
  uint16_t l = (k * (x & 255)) >> 8;
  return (h + l);
}

/**
 * Linear interpolation between `a` and `b`.
 *
 * \param k  an 8 bit coefficient (0..255 means 0..255/256)
 * \param a  a 16 bit unsigned integer
 * \param b  the other 16 bit unsigned integer
 * \return   the integer part of the result.
 */
inline uint16_t lerpQ8(uint8_t k, uint16_t a, uint16_t b) {
  return !k ? a : (kxQ8(256 - k, a) + kxQ8(k, b));
}

/**
 * \param pitch   pitch (pitch == 128 means +1 note)
 * \return        OSC period (≈ wave lengh)
 */
int16_t snd_osc_period(int16_t pitch) {
  if (pitch < (MIDI_NOTE_NUMBER_MIN << 8)) {
    // out of range (illegal MIDI note number)
    return osc_periods[0];
  }
  if ((MIDI_NOTE_NUMBER_MAX << 8) <= pitch) {
    // out of range (illegal MIDI note number)
    return osc_periods[OSC_PERIODS_INDEX_MAX - 1];
  }
  int8_t idx = MIDI_NOTE_NUMBER_TO_INDEX(pitch >> 8);
  return lerpQ8(pitch & 255, osc_periods[idx], osc_periods[idx+1]);
}
