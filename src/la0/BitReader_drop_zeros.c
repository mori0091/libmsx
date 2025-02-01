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
 * \file BitReader_drop_zeros.c
 */

#include "BitReader.h"
#include "bits.h"

uint8_t BitReader_drop_zeros(struct BitReader * in) {
  uint8_t n = 0;
  uint8_t x;
  if (in->nbits) {
    x = in->bits;
    n = bits_count_leading_zeros_u8(x);
    if (n < in->nbits) {
      in->nbits -= n;
      in->bits = x << n;
      return n;
    }
    n = in->nbits;
    in->nbits = 0;
  }
  while (!(x = mfread_u8(&in->mf))) {
    n += 8;
  }
  uint8_t m = bits_count_leading_zeros_u8(x);
  in->nbits = 8 - m;
  in->bits = x << m;
  return n + m;
}
