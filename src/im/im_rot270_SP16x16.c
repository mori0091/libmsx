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
 * \file im_rot270_SP16x16.c
 */

#include <im.h>

void im_rot270_SP16x16(const SP16x16 * src, SP16x16 * dst) {
  im_rot270_BM8x8(&(*src)[2], &(*dst)[0]);
  im_rot270_BM8x8(&(*src)[0], &(*dst)[1]);
  im_rot270_BM8x8(&(*src)[3], &(*dst)[2]);
  im_rot270_BM8x8(&(*src)[1], &(*dst)[3]);
}
