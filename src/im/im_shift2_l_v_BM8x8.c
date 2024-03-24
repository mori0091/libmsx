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
 * \file im_shift2_l_v_BM8x8.c
 */

#include <im.h>
#include "im_shift2.h"

#include <vmem.h>

void im_shift2_l_v_BM8x8(uint8_t sft, const BM8x8 * src, const BM8x8 * oth) {
  sft &= 7;
  const uint8_t * p = &(*src)[0];
  const uint8_t * o = &(*oth)[0];
  for (uint8_t n = 8; n--; ) {
    vmem_set((*p++ << sft) | (*o++ >> (8 - sft)));
  }
}
