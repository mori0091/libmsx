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
 * \file scc.c
 */

#include "scc.h"
#include "scc_io.h"

#include "bios.h"
#include "slot.h"

static void unexpose_SCC(uint8_t slot) {
  const uint8_t slot_p2 = msx_get_slot(PAGE_ADDR(2));
  msx_ENASLT(slot, PAGE_ADDR(2));
  {
    SCC_BANK_SELECT_3 = 0x00;
    SCCPlus_mode_select = 0x00;
    SCC_BANK_SELECT_2 = 0x00;
  }
  msx_ENASLT(slot_p2, PAGE_ADDR(2));
  __asm__("ei");
}
static void expose_SCC(uint8_t slot) {
  const uint8_t slot_p2 = msx_get_slot(PAGE_ADDR(2));
  msx_ENASLT(slot, PAGE_ADDR(2));
  {
    SCC_BANK_SELECT_3 = 0x00;
    SCCPlus_mode_select = 0x00;
    SCC_BANK_SELECT_2 = 0x3f;
  }
  msx_ENASLT(slot_p2, PAGE_ADDR(2));
  __asm__("ei");
}
static void expose_SCCPlus(uint8_t slot) {
  const uint8_t slot_p2 = msx_get_slot(PAGE_ADDR(2));
  msx_ENASLT(slot, PAGE_ADDR(2));
  {
    SCC_BANK_SELECT_3 = 0x00;
    SCCPlus_mode_select = 0x20;
    SCC_BANK_SELECT_3 = 0x80;
  }
  msx_ENASLT(slot_p2, PAGE_ADDR(2));
  __asm__("ei");
}

uint8_t SCC_inspect(uint8_t slot) {
  if (slot_is_MAIN_ROM(slot) ||
      slot_is_SUB_ROM(slot)  ||
      slot_is_BDOS(slot)     ||
      slot_is_OPLL(slot)) {
    return 0;
  }
  if (!slot_is_read_only(slot, (void *)0x8000)) {
    return 0;
  }

  unexpose_SCC(slot);
  if (!slot_is_read_only(slot, (void *)SCC_waveform)) {
    return 0;
  }

  expose_SCC(slot);
  if (slot_is_read_only(slot, (void *)SCC_waveform)) {
    return 0;
  }

  expose_SCCPlus(slot);
  uint8_t ver;
  if (slot_is_read_only(slot, (void *)SCCPlus_waveform)) {
    ver = 1;                    // SCC MegaROM
  }
  else {
    ver = 2;                    // SCC+ Sound Cartridge
  }

  unexpose_SCC(slot);
  return ver;
}

extern const struct SCC_Device SCC_device;
extern const struct SCC_Device SCCPlus_device;

static void SCC_find_callback(uint8_t slot, void * arg) {
  uint8_t x = SCC_inspect(slot);
  if (!x) return;

  struct SCC * scc = (struct SCC *)arg;
  if (!scc->slot || scc->version < x) {
    scc->slot = slot;
    scc->version = x;           // 1: SCC, 2: SCC+
    scc->mode = 1;
    scc->device = &SCC_device;
  }
}

uint8_t SCC_find(struct SCC * scc) {
  if (!scc) return 0;
  scc->slot = 0;
  scc->version = 0;
  slot_iterate(SCC_find_callback, scc);
  return scc->slot;
}

void SCC_set_mode(struct SCC * scc, uint8_t mode) {
  if (!scc || scc->version < 2) return;
  if (mode == 1) {
    scc->mode = 1;
    scc->device = &SCC_device;
  }
  else if (mode == 2) {
    scc->mode = 2;
    scc->device = &SCCPlus_device;
  }
}

uint8_t SCC_get_mode(struct SCC * scc) {
  if (!scc) return 0;
  return scc->mode;
}

void SCC_enable(const struct SCC * scc) {
  if (scc && scc->slot) {
    if (scc->mode == 1) {
      // Expose SCC on the `scc->slot`
      expose_SCC(scc->slot);
    }
    else if (scc->mode == 2) {
      // Expose SCC+ on the `scc->slot`
      expose_SCCPlus(scc->slot);
    }
  }
}

void SCC_disable(const struct SCC * scc) {
  if (scc && scc->slot) {
    // Unexpose SCC/SCC+ on the `scc->slot`
    unexpose_SCC(scc->slot);
  }
}
