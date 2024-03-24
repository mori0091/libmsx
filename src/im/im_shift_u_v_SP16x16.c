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
 * \file im_shift_u_v_SP16x16.c
 */

#include <im.h>
#include <vmem.h>

void im_shift_u_v_SP16x16(uint8_t sft, const SP16x16 * src) {
  if (16 <= sft) sft = 16;
  {
    const uint8_t * p = &(*src)[0][sft];
    for (uint8_t n = 16 - sft; n--; ) {
      vmem_set(*p++);
    }
    for (uint8_t n = sft; n--; ) {
      vmem_set(0);
    }
  }
  {
    const uint8_t * p = &(*src)[2][sft];
    for (uint8_t n = 16 - sft; n--; ) {
      vmem_set(*p++);
    }
    for (uint8_t n = sft; n--; ) {
      vmem_set(0);
    }
  }
}
