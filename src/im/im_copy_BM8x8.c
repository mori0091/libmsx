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
 * \file im_copy_BM8x8.c
 */

#include <im.h>

#include <string.h>

void im_copy_BM8x8(const BM8x8 * src, BM8x8 * dst) {
  memcpy(dst, src, 8);
}
