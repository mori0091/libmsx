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
 * \file im_tr_BM8x8.c
 */

#include <im.h>

#include "im_tr.h"

void im_tr_BM8x8(const BM8x8 * src, BM8x8 * dst) {
  const uint8_t * const p = *src;
  uint8_t * q = &(*dst)[0];
  *q++ = im_tr7(p);
  *q++ = im_tr6(p);
  *q++ = im_tr5(p);
  *q++ = im_tr4(p);
  *q++ = im_tr3(p);
  *q++ = im_tr2(p);
  *q++ = im_tr1(p);
  *q   = im_tr0(p);
}
