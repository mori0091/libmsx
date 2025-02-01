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
 * \file im_rotate_u_SP16x16.c
 */

#include <im.h>

#include <string.h>

void im_rotate_u_SP16x16(uint8_t sft, const SP16x16 * src, SP16x16 * dst) {
  sft &= 15;
  if (!sft) {
    memcpy(dst, src, 32);
  }
  else {
    const uint8_t a = 16 - sft;
    memcpy((uint8_t *)*dst         , (uint8_t *)*src      + sft, a);
    memcpy((uint8_t *)*dst      + a, (uint8_t *)*src           , sft);
    memcpy((uint8_t *)*dst + 16    , (uint8_t *)*src + 16 + sft, a);
    memcpy((uint8_t *)*dst + 16 + a, (uint8_t *)*src + 16      , sft);
  }
}
