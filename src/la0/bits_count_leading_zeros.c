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
 * \file bits_count_leading_zeros_u8.c
 */

#include "bits.h"

uint8_t bits_count_leading_zeros_u8(uint8_t x) {
  if (!x) return 8;
  uint8_t c = 0;
  if (!(x & 0xf0)) { x <<= 4; c += 4; }
  if (!(x & 0xc0)) { x <<= 2; c += 2; }
  if (!(x & 0x80)) { x <<= 1; c += 1; }
  return c;
}
