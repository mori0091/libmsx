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
 * \file im_rotate_r_vec_BM8x8.c
 */

#include <im.h>
#include "im_shift2.h"

void im_rotate_r_vec_BM8x8(uint8_t sft, size_t n, const BM8x8 * src, BM8x8 * dst) {
  src += n - 1;
  dst += n - 1;
  BM8x8 srcN;
  im_copy_BM8x8(src, &srcN);
  while (1 < n--) {
    im_shift2_r_BM8x8(sft, src - 1, src, dst--);
    src--;
  }
  im_shift2_r_BM8x8(sft, &srcN, src, dst);
}
