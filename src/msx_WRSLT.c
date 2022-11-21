// -*- coding: utf-8-unix -*-
/**
 * \file msx_WRSLT.c
 *
 * Copyright (c) 2022 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */

#include "../include/bios.h"

#ifndef __SDCCCALL
#define __sdcccall(x)
#endif

void msx_WRSLT(uint8_t slot, void * addr, uint8_t value) __sdcccall(0) __naked {
  (void)slot;                   // (SP+2) --> A
  (void)addr;                   // (SP+3) --> HL
  (void)value;                  // (SP+5) --> E
  __asm__("push af");
  __asm__("push bc");
  __asm__("push de");
  __asm__("push hl");
  __asm__("push ix");

  __asm__("ld ix, #10");
  __asm__("add ix, sp");
  __asm__("ld a, 2 (ix)");
  __asm__("ld l, 3 (ix)");
  __asm__("ld h, 4 (ix)");
  __asm__("ld e, 5 (ix)");

  __asm__("call _WRSLT");

  __asm__("pop ix");
  __asm__("pop hl");
  __asm__("pop de");
  __asm__("pop bc");
  __asm__("pop af");

  __asm__("ret");
}
