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
 * \file bmem_bload.c
 */

#include <bmem.h>

void bmem_bload(bmemptr_t src, void * buf, size_t buf_size) {
  if (bmem_get(src) == 0xfe) {
    uint16_t beg = bmem_get_u16(src+1);
    uint16_t end = bmem_get_u16(src+3);
    if (beg < end) {
      const size_t len = end - beg + 1;
      if (len <= buf_size) {
        bmem_read(src+7, buf, len);
      }
    }
  }
}
