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
 * \file im_xor_BM8x8.c
 */

#include <im.h>

void im_xor_BM8x8(const BM8x8 * src1, const BM8x8 * src2, BM8x8 * dst) {
  const uint8_t * p1 = &(*src1)[0];
  const uint8_t * p2 = &(*src2)[0];
  uint8_t * q = &(*dst)[0];
  for (uint8_t n = 8; n--; ) {
    *q++ = *p1++ ^ *p2++;
  }
}
