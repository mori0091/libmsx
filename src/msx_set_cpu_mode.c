// -*- coding: utf-8-unix -*-
/**
 * \file msx_set_cpu_mode.c
 *
 * Copyright (c) 2021 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

#include "../include/bios.h"

static __at(0x0180) uint8_t CHGCPU[0];

void msx_set_cpu_mode(const uint8_t mode) {
  ((void)mode);
  if (msx_get_version() < 3) return;
  __asm__("ld hl, #2+0");
  __asm__("add hl, sp");
  __asm__("ld a, (hl)");
  __asm__("and a, #0x83");
  __asm__("jp _CHGCPU");
}
