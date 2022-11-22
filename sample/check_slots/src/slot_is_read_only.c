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
 * \file slot_is_read_only.c
 */

#include "slot.h"

#include <bios.h>

bool slot_is_read_only(uint8_t slot, void * addr) {
  const uint8_t x = msx_RDSLT(slot, addr);
  msx_WRSLT(slot, addr, ~x);
  if (!(x ^ msx_RDSLT(slot, addr))) {
    __asm__("ei");
    return true;
  }
  msx_WRSLT(slot, addr, x);
  __asm__("ei");
  return false;
}
