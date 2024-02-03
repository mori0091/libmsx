// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2024 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file BitReader.c
 */

#include "BitReader.h"
#include <stdint.h>

bool read_bit(struct BitReader * in) {
  if (!(in->bitmask >>= 1)) {
    in->bitmask = 0x80;
    in->bits = mfread_u8(&in->mf);
  }
  return (in->bits & in->bitmask);
}

uint8_t count_zeros(struct BitReader * in) {
  uint8_t n = 0;
  for (;;) {
    if (!(in->bitmask >>= 1)) {
      in->bitmask = 0x80;
      while (!(in->bits = mfread_u8(&in->mf))) {
        n += 8;
      }
    }
    if (in->bits & in->bitmask) {
      return n;
    }
    n++;
  }
}

uint8_t elias_gamma_u8(struct BitReader * in) {
  uint8_t x = 1;
  for (uint8_t nbits = count_zeros(in); nbits; nbits--) {
    if (!(in->bitmask >>= 1)) {
      in->bitmask = 0x80;
      in->bits = mfread_u8(&in->mf);
    }
    x <<= 1;
    if (in->bits & in->bitmask) {
      x |= 1;
    }
  }
  return x;
}

uint32_t elias_gamma_u32(struct BitReader * in) {
  uint32_t x = 1;
  for (uint8_t nbits = count_zeros(in); nbits; nbits--) {
    if (!(in->bitmask >>= 1)) {
      while (8 <= nbits) {
        nbits -= 8;
        x = (x << 8) | mfread_u8(&in->mf);
      }
      if (!nbits) break;
      in->bitmask = 0x80;
      in->bits = mfread_u8(&in->mf);
    }
    x <<= 1;
    if (in->bits & in->bitmask) {
      x |= 1;
    }
  }
  return x;
}
