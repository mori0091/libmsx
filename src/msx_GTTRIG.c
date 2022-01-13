// -*- coding: utf-8-unix -*-
/**
 * \file msx_GTTRIG.c
 *
 * Copyright (c) 2022 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

#include "../include/bios.h"

uint8_t msx_GTTRIG(const uint8_t a) __naked {
  ((void)a);
  __asm__("ld   hl, #2");
  __asm__("add  hl, sp");
  __asm__("ld   a, (hl)");
  __asm__("call _GTTRIG");
  __asm__("ld   l, a");
  __asm__("ret");
}
