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
 * \file scc.c
 */

#include <msx.h>
#include <stdint.h>

#include "scc.h"

extern int slot_bcmp(uint8_t slot, const void * addr, const void * s, size_t len);
extern void foreach_slot(void (*callback)(uint8_t slot, void * arg), void * arg);

extern bool is_read_only(uint8_t slot, void * addr);

uint8_t SCC_inspect(uint8_t slot) {
  if (!is_read_only(slot, (void *)0x8000)) {
    return 0;
  }
  msx_WRSLT(slot, (void *)&SCCPlus_mode_select, 0x00);
  msx_WRSLT(slot, (void *)&SCC_BANK_SELECT_2, 0x00);
  if (!is_read_only(slot, (void *)SCC_waveform)) {
    return 0;
  }
  msx_WRSLT(slot, (void *)&SCC_BANK_SELECT_2, 0x3f);
  if (is_read_only(slot, (void *)SCC_waveform)) {
    return 0;
  }
  msx_WRSLT(slot, (void *)&SCCPlus_mode_select, 0x20);
  msx_WRSLT(slot, (void *)&SCC_BANK_SELECT_3, 0x80);
  if (is_read_only(slot, (void *)SCCPlus_waveform)) {
    return 1;                   // SCC MegaROM
  }
  return 2;                     // SCC+ Sound Cartridge
}

static void SCC_find_callback(uint8_t slot, void * arg) {
  uint8_t x = SCC_inspect(slot);
  if (!x) return;

  struct SCC * scc = (struct SCC *)arg;
  scc->slot = slot;
  scc->version = x;             // 1: SCC, 2: SCC+
}

void SCC_find(struct SCC * scc) {
  scc->slot = 0;
  scc->version = 0;
  foreach_slot(SCC_find_callback, scc);
}
