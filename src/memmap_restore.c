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
 * \file memmap_restore.c
 */

#include "./memmap.h"

void memmap_restore(const struct MemMap * ctx) {
  msx_ENASLT(ctx->slot_p1, PAGE_ADDR(1));
  msx_ENASLT(ctx->slot_p2, PAGE_ADDR(2));
  if (ctx->interrupt_enabled) {
    __asm__("ei");
  }
}
