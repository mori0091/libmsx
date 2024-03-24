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
 * \file im_shift_u_SP16x16.c
 */

#include <im.h>

#include <string.h>

void im_shift_u_SP16x16(uint8_t sft, const SP16x16 * src, SP16x16 * dst) {
  if (16 <= sft) sft = 16;
  const uint8_t a = 16 - sft;
  memcpy((uint8_t *)*dst         , (uint8_t *)*src      + sft, a);
  memset((uint8_t *)*dst      + a, 0, sft);
  memcpy((uint8_t *)*dst + 16    , (uint8_t *)*src + 16 + sft, a);
  memset((uint8_t *)*dst + 16 + a, 0, sft);
}
