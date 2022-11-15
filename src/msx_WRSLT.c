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

#if (__SDCCCALL == 1)

void msx_WRSLT(uint8_t slot, void * addr, uint8_t value) __naked {
  (void)slot;                   // A      --> A
  (void)addr;                   // DE     --> HL
  (void)value;                  // (SP+2) --> E
  __asm__("ld hl, #2");
  __asm__("add hl, sp");
  __asm__("ld l, (hl)");
  __asm__("ex de, hl");
  __asm__("jp _WRSLT");
}

#else

void msx_WRSLT(uint8_t slot, void * addr, uint8_t value) __naked {
  (void)slot;                   // (SP+2) --> A
  (void)addr;                   // (SP+3) --> HL
  (void)value;                  // (SP+5) --> E
  __asm__("push ix");
  __asm__("ld ix, #0");
  __asm__("add ix, sp");
  __asm__("ld a, 4 (ix)");
  __asm__("ld l, 5 (ix)");
  __asm__("ld h, 6 (ix)");
  __asm__("ld e, 7 (ix)");
  __asm__("pop ix");
  __asm__("jp _WRSLT");
}

#endif
