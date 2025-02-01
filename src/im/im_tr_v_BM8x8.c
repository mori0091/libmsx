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
 * \file im_tr_BM8x8.c
 */

#include <im.h>
#include <vmem.h>

#include "im_tr.h"

void im_tr_v_BM8x8(const BM8x8 * src) {
  const uint8_t * const p = *src;
  vmem_set(im_tr7(p));
  vmem_set(im_tr6(p));
  vmem_set(im_tr5(p));
  vmem_set(im_tr4(p));
  vmem_set(im_tr3(p));
  vmem_set(im_tr2(p));
  vmem_set(im_tr1(p));
  vmem_set(im_tr0(p));
}
