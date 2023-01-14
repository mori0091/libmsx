// -*- coding: utf-8-unix -*-
/**
 * \file msx_RDSLT.c
 *
 * Copyright (c) 2021-2023 Daishi Mori (mori0091)
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
  __asm__("push bc");
  __asm__("push de");
  __asm__("push hl");

  __asm__("ld h, d");
  __asm__("ld l, e");

  __asm__("call _RDSLT");

  __asm__("pop hl");
  __asm__("pop de");
  __asm__("pop bc");

  __asm__("ret");
}

#else

uint8_t msx_RDSLT(uint8_t slot, void * addr) __naked {
  (void)slot;                   // (SP+2) --> A
  (void)addr;                   // (SP+3) --> HL
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

  __asm__("call _RDSLT");

  __asm__("pop ix");
  __asm__("pop hl");
  __asm__("ld l, a");
  __asm__("pop de");
  __asm__("pop bc");
  __asm__("pop af");

  __asm__("ret");
}

#endif
