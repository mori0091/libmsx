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
 * \file im_rot90_v_BM8x8.c
 */

#include <im.h>
#include <vmem.h>

#include "im_adtr.h"

void im_rot90_v_BM8x8(const BM8x8 * src) {
  const uint8_t * const p = *src;
  vmem_set(im_adtr7(p));
  vmem_set(im_adtr6(p));
  vmem_set(im_adtr5(p));
  vmem_set(im_adtr4(p));
  vmem_set(im_adtr3(p));
  vmem_set(im_adtr2(p));
  vmem_set(im_adtr1(p));
  vmem_set(im_adtr0(p));
}
