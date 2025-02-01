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
 * \file im_rot180_v_SP16x16.c
 */

#include <im.h>

void im_rot180_v_SP16x16(const SP16x16 * src) {
  im_rot180_v_BM8x8(&(*src)[3]);
  im_rot180_v_BM8x8(&(*src)[2]);
  im_rot180_v_BM8x8(&(*src)[1]);
  im_rot180_v_BM8x8(&(*src)[0]);
}
