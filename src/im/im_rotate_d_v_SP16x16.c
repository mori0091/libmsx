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
 * \file im_rotate_d_v_SP16x16.c
 */

#include <im.h>
#include <vmem.h>

void im_rotate_d_v_SP16x16(uint8_t sft, const SP16x16 * src) {
  sft &= 15;
  {
    const uint8_t * p = &(*src)[0][16 - sft];
    for (uint8_t n = sft; n--; ) {
      vmem_set(*p++);
    }
  }
  {
    const uint8_t * p = &(*src)[0][0];
    for (uint8_t n = 16 - sft; n--; ) {
      vmem_set(*p++);
    }
  }
  {
    const uint8_t * p = &(*src)[2][16 - sft];
    for (uint8_t n = sft; n--; ) {
      vmem_set(*p++);
    }
  }
  {
    const uint8_t * p = &(*src)[2][0];
    for (uint8_t n = 16 - sft; n--; ) {
      vmem_set(*p++);
    }
  }
}
