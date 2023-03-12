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
 * \file bmem_read.c
 */

#if 0

#include "bmem.h"
#include "workarea.h"

#include <stdint.h>
#include <string.h>

void bmem_read(bmemptr_t src, void* dst, uint16_t len) {
  uint8_t * q = (uint8_t *)dst;

  // Overflow check.
  //
  // \note
  // Page 0..2 (0x0000..0xbfff) are ROM.
  //
  // \note
  // Address 0xffff is not memory, that is "extended slot selector" register.
  //
  // \attention
  // Stack areas and work areas are overridden if they intersect the destination
  // area specified by `dst` and `len`. Unfortunately, however, the library
  // cannot determine the appropriate bounds. The application programmer must
  // deal with this.
  if ((uintptr_t)q < 0xc000 || HIMEM <= (uintptr_t)q || (int16_t)(HIMEM - (uintptr_t)q) < len) {
    return;
  }

  uint8_t bank = bmem_bank_of(src);
  const uint16_t offset = (uint16_t)(src & 0x3fff);
  const uint8_t * p = (const uint8_t *)0x8000 + offset;
  uint16_t n = (uint16_t)0x4000 - offset;

  const uint8_t old_bank = bmem_get_bank();
  while (len) {
    if (len < n) {
      n = len;
    }
    len -= n;
    bmem_set_bank(bank);
    memcpy(q, p, n);
    q += n;
    bank++;
    p = (const uint8_t *)0x8000;
    n = 0x4000;
  }
  bmem_set_bank(old_bank);
}

#else

#include "bmem.h"
#include "xmem.h"

void bmem_read(bmemptr_t loc, void * dst, uint16_t len) {
  mem_open((uint8_t *)dst);
  bmem_open(loc);
  bmem_to_mem(len);
  bmem_close();
  mem_close();
}

#endif
