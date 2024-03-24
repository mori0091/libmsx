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
 * \file im_rot180_v_BM8x8.c
 */

#include <im.h>
#include <vmem.h>

void im_rot180_v_BM8x8(const BM8x8 * src) {
  const uint8_t * p = &(*src)[7];
  for (uint8_t n = 8; n--; ) {
    uint8_t x = *p--;
    x = ((x << 1) & 0xaa) | ((x >> 1) & 0x55);
    x = ((x << 2) & 0xcc) | ((x >> 2) & 0x33);
    x = ((x << 4) & 0xf0) | ((x >> 4) & 0x0f);
    vmem_set(x);
  }
}
