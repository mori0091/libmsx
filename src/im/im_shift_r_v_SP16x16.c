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
 * \file im_shift_r_v_SP16x16.c
 */

#include <im.h>
#include <vmem.h>

void im_shift_r_v_SP16x16(uint8_t sft, const SP16x16 * src) {
  if (16 <= sft) sft = 16;
  const uint8_t sft2 = (8 < sft) ? 8 : sft;
  const uint8_t * p = &(*src)[0][0];
  for (uint8_t n = 16; n--; ) {
    vmem_set(*p++ >> sft2);
  }
  for (uint8_t n = 16; n--; ) {
    uint16_t a = (((uint16_t)p[-16] << 8) | p[0]) >> sft;
    vmem_set(a & 255);
    p++;
  }
}
