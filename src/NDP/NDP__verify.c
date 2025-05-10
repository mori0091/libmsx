// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2025 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file NDP__verify.c
 */

#include "internal/memfile_types.h"
#include <memfile.h>
#include <stdint.h>

#define MIN(a, b)  ((a) <= (b) ? (a) : (b))
#define MAX(a, b)  ((b) <= (a) ? (a) : (b))

static int NDP__verify0(MemFile * mf) {
  if (!mf) return -1;

  size_t min_offset = SIZE_MAX;
  size_t max_offset = 0;

  // read 4 tracks' offset
  for (uint8_t n = 4; n--; ) {
    size_t o = mfread_u16(mf);
    if (0x0008 <= o) {
      min_offset = MIN(min_offset, o);
      max_offset = MAX(max_offset, o);
    }
  }

  // read voice track's offset
  if (0x000e <= min_offset) {
    size_t o = mfread_u16(mf);
    min_offset = MIN(min_offset, o);
    max_offset = MAX(max_offset, o);
  }
  if (mfsize(mf) <= max_offset) return -1; // Invalid offset
  if (max_offset < min_offset) return -1;  // Invalid offset
  if (min_offset < 0x000e) return -1;      // Not a song (maybe a SFX)

  // search end of the last track
  mfseek(mf, max_offset, MEM_SEEK_SET);
  for (;;) {
    if (mfeof(mf)) return -1;   // Invalid data
    if (mfread_u8(mf) == 0xFF) break;
  }

  return 1;
}

int NDP__verify(MemFile * mf) {
  mfseek(mf, 0, MEM_SEEK_SET);
  int num_songs = 0;
  while (0 < NDP__verify0(mf)) {
    num_songs++;
  }
  return num_songs;
}
