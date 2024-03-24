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
 * \file im_copy_v_BM8x8.c
 */

#include <im.h>
#include <vmem.h>

void im_copy_v_BM8x8(const BM8x8 * src) {
  const uint8_t * p = &(*src)[0];
  for (uint8_t n = 8; n--; ) {
    vmem_set(*p++);
  }
}
