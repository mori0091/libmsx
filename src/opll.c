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
 * \file opll.h
 * \brief Device interface for MSX-MUSIC (OPLL).
 */

#include "opll.h"

#include "slot.h"
#include "bios.h"

uint8_t OPLL_inspect(uint8_t slot) {
  if (slot_is_internal_OPLL(slot)) {
    return 0x80;
  }
  if (slot_is_FMPAC(slot)) {
    return 0x01;
  }
  if (slot_is_OPLL(slot)) {
    return 0x02;
  }
  return 0;
}

extern const struct OPLL_Device OPLL_device_turboR;
extern const struct OPLL_Device OPLL_device;

static void OPLL_find_callback(uint8_t slot, void * arg) {
  uint8_t ver = OPLL_inspect(slot);
  if (!ver) {
    return;
  }
  struct OPLL * opll = (struct OPLL *)arg;
  if (!opll->slot || opll->version < ver) {
    opll->slot = slot;
    opll->version = ver;
    if (3 <= msx_get_version()) {
      opll->device = &OPLL_device_turboR;
    }
    else {
      opll->device = &OPLL_device;
    }
  }
}

uint8_t OPLL_find(struct OPLL * opll) {
  if (!opll) return 0;
  opll->slot = 0;
  opll->version = 0;
  slot_iterate(OPLL_find_callback, opll);
  return opll->slot;
}

void OPLL_enable(const struct OPLL * opll) {
  if (opll && opll->slot) {
    if (!(opll->version & 0x80)) {
      // An external OPLL
      // Attach the external OPLL to Z80 I/O port.
      uint8_t x = msx_RDSLT(opll->slot, (void *)0x7ff6);
      msx_WRSLT(opll->slot, (void *)0x7ff6, x | 0x01);
      __asm__("ei");
    }
  }
}

void OPLL_disable(const struct OPLL * opll) {
  if (opll && opll->slot) {
    if (!(opll->version & 0x80)) {
      // An external OPLL
      // Dettach the external OPLL from Z80 I/O port.
      uint8_t x = msx_RDSLT(opll->slot, (void *)0x7ff6);
      msx_WRSLT(opll->slot, (void *)0x7ff6, x & 0xfe);
      __asm__("ei");
    }
  }
}
