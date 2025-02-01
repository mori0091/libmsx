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
 * \file bmem_get.c
 */

#include "bmem.h"

uint8_t bmem_get(bmemptr_t src) {
  const uint8_t old_bank = bmem_get_bank();
  uint8_t bank = bmem_bank_of(src);
  bmem_set_bank(bank);
  uint8_t ret = *(const uint8_t *)(const uintptr_t)((src & 0x3fff) + 0x8000);
  bmem_set_bank(old_bank);
  return ret;
}
