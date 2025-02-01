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
 * \file im_rotate_d_BM8x8.c
 */

#include <im.h>

#include <string.h>

void im_rotate_d_BM8x8(uint8_t sft, const BM8x8 * src, BM8x8 * dst) {
  sft &= 7;
  const uint8_t a = 8 - sft;
  memcpy(*dst + sft, *src, a);
  memcpy(*dst, *src + a, sft);
}
