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
 * \file memmap_expose_cartridge.c
 */

#include "./memmap.h"

void memmap_expose_cartridge(void) {
  msx_ENASLT(CARTRIDGE_SLOT, PAGE_ADDR(1));
  msx_ENASLT(CARTRIDGE_SLOT, PAGE_ADDR(2));
}
