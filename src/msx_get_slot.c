// -*- coding: utf-8-unix -*-
/**
 * \file msx_get_slot.c
 *
 * Copyright (c) 2022 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */

#include "../include/bios.h"
#include "../include/workarea.h"
#include <stdint.h>

uint8_t msx_get_slot(void * addr) {
  uint8_t page = (uint8_t)((uintptr_t)addr >> 14) & 3;
  __asm__("di");
  uint8_t slot = (msx_RSLREG() >> (page * 2)) & 3;
  if (EXPTBL[slot] & 0x80) {
    slot |= ((SLTTBL[slot] >> (page * 2)) & 3) << 2;
    slot |= 0x80;
  }
  __asm__("ei");
  return slot;
}
