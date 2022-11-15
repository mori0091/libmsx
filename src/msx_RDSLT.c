// -*- coding: utf-8-unix -*-
/**
 * \file msx_RDSLT.c
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

uint8_t msx_RDSLT(uint8_t slot, void * addr) __naked {
  (void)slot;                   // A  --> A
  (void)addr;                   // DE --> HL
  __asm__("ld h, d");
  __asm__("ld l, e");
  __asm__("jp _RDSLT");
}

#else

uint8_t msx_RDSLT(uint8_t slot, void * addr) __naked {
  (void)slot;                   // (SP+2) --> A
  (void)addr;                   // (SP+3) --> HL
  __asm__("push ix");
  __asm__("ld ix, #0");
  __asm__("add ix, sp");
  __asm__("ld a, 4 (ix)");
  __asm__("ld l, 5 (ix)");
  __asm__("ld h, 6 (ix)");
  __asm__("pop ix");
  __asm__("call _RDSLT");
  __asm__("ld l, a");
  __asm__("ret");
}

#endif
