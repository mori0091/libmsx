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
 * \file memmap_save.c
 */

#include "./memmap.h"

void memmap_save(struct MemMap * ctx) {
  ctx->interrupt_enabled = get_interrupt_state();
  ctx->slot_p1 = msx_get_slot(PAGE_ADDR(1));
  ctx->slot_p2 = msx_get_slot(PAGE_ADDR(2));
}
