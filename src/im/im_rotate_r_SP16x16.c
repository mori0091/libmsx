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
 * \file im_rotate_r_SP16x16.c
 */

#include <im.h>

#include <string.h>

void im_rotate_r_SP16x16(uint8_t sft, const SP16x16 * src, SP16x16 * dst) {
  sft &= 15;
  if (!sft) {
    memcpy(dst, src, 32);
  }
  else {
    const uint8_t * p = &(*src)[0][0];
    uint8_t * q = &(*dst)[0][0];
    for (uint8_t n = 16; n--; ) {
      uint16_t x = (((uint16_t)p[0] << 8) | p[16]);
      uint16_t a = (x >> sft) | (x << (16 - sft));
      q[0] = a >> 8;
      q[16] = a & 255;
      p++;
      q++;
    }
  }
}
