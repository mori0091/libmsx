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
 * \file BitReader_read_elias_gamma_u32.c
 */

#include "BitReader.h"

uint32_t BitReader_read_elias_gamma_u32(struct BitReader * in) {
  uint32_t x = 0;
  for (uint8_t c = BitReader_drop_zeros(in) + 1; c; ) {
    if (!in->nbits) {
      while (8 <= c) {
        c -= 8;
        x = (x << 8) | mfread_u8(&in->mf);
      }
      if (!c) break;
      in->nbits = 8;
      in->bits = mfread_u8(&in->mf);
    }
    if (c <= in->nbits) {
      x = (x << c) | (in->bits >> (8 - c));
      in->nbits -= c;
      in->bits <<= c;
      break;
    }
    else {
      x = (x << in->nbits) | (in->bits >> (8 - in->nbits));
      c -= in->nbits;
      in->nbits = 0;
    }
  }
  return x;
}
