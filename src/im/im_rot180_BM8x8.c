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
 * \file im_rot180_BM8x8.c
 */

#include <im.h>

void im_rot180_BM8x8(const BM8x8 * src, BM8x8 * dst) {
  im_vflip_BM8x8(src, dst);
  im_hflip_BM8x8(dst, dst);
}
