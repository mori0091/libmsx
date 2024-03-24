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
 * \file im_vflip_v_SP16x16.c
 */

#include <im.h>

void im_vflip_v_SP16x16(const SP16x16 * src) {
  im_vflip_v_BM8x8(&(*src)[1]);
  im_vflip_v_BM8x8(&(*src)[0]);
  im_vflip_v_BM8x8(&(*src)[3]);
  im_vflip_v_BM8x8(&(*src)[2]);
}
