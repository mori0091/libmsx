// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2023 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file bmem_bload_s.c
 */

#include "bmem.h"

void bmem_bload_s(bmemptr_t src) {
  if (bmem_get(src) == 0xfe) {
    uint16_t beg = bmem_get_u16(src+1);
    uint16_t end = bmem_get_u16(src+3);
    if (beg < end) {
      bmem_copy_to_vmem(src+7, (vmemptr_t)beg, (uint32_t)end - beg + 1);
    }
  }
}
