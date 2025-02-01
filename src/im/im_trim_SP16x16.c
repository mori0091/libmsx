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
 * \file im_trim_SP16x16.c
 */

#include <im.h>

#include <string.h>

void im_trim_SP16x16(uint8_t tblr[4], const SP16x16 * src, SP16x16 * dst) {
  uint8_t t = tblr[0]; if (16 <= t) t = 16;
  uint8_t b = tblr[1]; if (16 <= b) b = 16;
  uint8_t l = tblr[2]; if (16 <= l) l = 16;
  uint8_t r = tblr[3]; if (16 <= r) r = 16;
  if (16 <= t + b || 16 <= l + r) {
    memset(dst, 0, sizeof(*dst));
    return;
  }

  const uint16_t mask = (0xffff >> l) & (0xffff << r);
  const uint8_t * p = &(*src)[0][0];
  uint8_t * q = &(*dst)[0][0];
  memset(q, 0, t);
  p += t; q += t;
  for (uint8_t n = 16 - t - b; n--; ) {
    *q++ = *p++ & (uint8_t)(mask >> 8);
  }
  memset(q, 0, t + b);
  p += b + t; q += b + t;
  for (uint8_t n = 16 - t - b; n--; ) {
    *q++ = *p++ & (uint8_t)(mask & 255);
  }
  memset(q, 0, b);
}
