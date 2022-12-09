// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2022 Daishi Mori (mori0091)
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

#include "bmem.h"

#include <string.h>

void bmem_read(bmemptr_t src, void* dst, uint16_t len) {
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
    memcpy(dst, p, n);
    dst = (uint8_t *)dst + n;
    bank++;
    p = (const uint8_t *)0x8000;
    n = 0x4000;
  }
  bmem_set_bank(old_bank);
}
