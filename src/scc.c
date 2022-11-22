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

#include "scc.h"
#include "scc_io.h"

#include "bios.h"
#include "slot.h"

static void unexpose_SCCPlus(uint8_t slot) {
  //msx_WRSLT(slot, (void *)&SCCPlus_mode_select, 0x20);
  msx_WRSLT(slot, (void *)&SCC_BANK_SELECT_3, 0x00);
}
static void unexpose_SCC(uint8_t slot) {
  unexpose_SCCPlus(slot);
  msx_WRSLT(slot, (void *)&SCCPlus_mode_select, 0x00);
  msx_WRSLT(slot, (void *)&SCC_BANK_SELECT_2, 0x00);
}
static void expose_SCC(uint8_t slot) {
  unexpose_SCCPlus(slot);
  msx_WRSLT(slot, (void *)&SCCPlus_mode_select, 0x00);
  msx_WRSLT(slot, (void *)&SCC_BANK_SELECT_2, 0x3f);
}
static void expose_SCCPlus(uint8_t slot) {
  unexpose_SCCPlus(slot);
  msx_WRSLT(slot, (void *)&SCCPlus_mode_select, 0x20);
  msx_WRSLT(slot, (void *)&SCC_BANK_SELECT_3, 0x80);
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
    if (1 < x) {
      scc->device = &SCCPlus_device;
    }
    else {
      scc->device = &SCC_device;
    }
  }
}

uint8_t SCC_find(struct SCC * scc) {
  if (!scc) return 0;
  scc->slot = 0;
  scc->version = 0;
  slot_iterate(SCC_find_callback, scc);
  return scc->slot;
}

void SCC_enable(const struct SCC * scc) {
  if (scc && scc->slot) {
    if (1 < scc->version) {
      // Expose SCC+ on the `scc->slot`
      expose_SCCPlus(scc->slot);
    }
    else {
      // Expose SCC on the `scc->slot`
      expose_SCC(scc->slot);
    }
  }
}

void SCC_disable(const struct SCC * scc) {
  if (scc && scc->slot) {
    // Unexpose SCC/SCC+ on the `scc->slot`
    unexpose_SCC(scc->slot);
  }
}
