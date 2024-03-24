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
 * \file im_rotate_l_vec_BM8x8.c
 */

#include <im.h>
#include "im_shift2.h"

void im_rotate_l_vec_BM8x8(uint8_t sft, size_t n, const BM8x8 * src, BM8x8 * dst) {
  BM8x8 src0;
  im_copy_BM8x8(src, &src0);
  while (1 < n--) {
    im_shift2_l_BM8x8(sft, src, src + 1, dst++);
    src++;
  }
  im_shift2_l_BM8x8(sft, src, &src0, dst);
}
