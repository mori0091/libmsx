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
 * \file memmap.h
 */

#include <bios.h>
#include <interrupt.h>
#include <stdint.h>

#ifndef MEMMAP_H_
#define MEMMAP_H_

struct MemMap {
  bool interrupt_enabled;
  uint8_t slot_p1;
  uint8_t slot_p2;
};

/**
 * Save current slots and interrupt state.
 *
 * \param ctx  Pointer to structure to save current state.
 */
void memmap_save(struct MemMap * ctx);

/**
 * Restore the saved slots and interrupt state.
 *
 * \param ctx  Pointer to the structure in which the previous state was saved.
 */
void memmap_restore(const struct MemMap * ctx);

/**
 * Expose the cartridge slots.
 *
 * \post Interupt is disabled.
 */
void memmap_expose_cartridge(void);

#endif // MEMMAP_H_
