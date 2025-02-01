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
 * \file im_shift_l_v_BM8x8.c
 */

#include <im.h>
#include <vmem.h>

void im_shift_l_v_BM8x8(uint8_t sft, const BM8x8 * src) {
  if (8 <= sft) sft = 8;
  const uint8_t * p = &(*src)[0];
  for (uint8_t n = 8; n--; ) {
    vmem_set(*p++ << sft);
  }
}
