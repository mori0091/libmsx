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
 * \file im_xor_SP16x16.c
 */

#include <im.h>

void im_xor_SP16x16(const SP16x16 * src1, const SP16x16 * src2, SP16x16 * dst) {
  const uint8_t * p1 = &(*src1)[0][0];
  const uint8_t * p2 = &(*src2)[0][0];
  uint8_t * q = &(*dst)[0][0];
  for (uint8_t n = 32; n--; ) {
    *q++ = *p1++ ^ *p2++;
  }
}
