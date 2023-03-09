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
 * \file bmem_copy_to_vmem.c
 */

#include "bmem.h"

void bmem_copy_to_vmem(bmemptr_t src, vmemptr_t dst, uint32_t len) {
  uint8_t bank = bmem_bank_of(src);
  const uint16_t offset = (uint16_t)(src & 0x3fff);
  const uint8_t * p = (const uint8_t *)0x8000 + offset;
  uint16_t n = (uint16_t)0x4000 - offset;

  const uint8_t old_bank = bmem_get_bank();
  vmem_set_write_address(dst);
  while (len) {
    if (len < n) {
      n = len;
    }
    len -= n;
    bmem_set_bank(bank);
    while (n--) {
      vmem_set(*p++);
    }
    bank++;
    p = (const uint8_t *)0x8000;
    n = (uint16_t)0x4000;
  }
  bmem_set_bank(old_bank);
}
