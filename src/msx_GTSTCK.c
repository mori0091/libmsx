// -*- coding: utf-8-unix -*-
/**
 * \file msx_GTSTCK.c
 *
 * Copyright (c) 2021-2025 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

#include "../include/bios.h"

#if (__SDCCCALL == 1)

uint8_t msx_GTSTCK(const uint8_t a) __naked {
  ((void)a);                    // A --> A
  __asm__("push bc");
  __asm__("push de");
  __asm__("push hl");
  __asm__("push ix");
  __asm__("push iy");

  __asm__("ex af, af'");
  __asm__("exx");
  __asm__("push af");
  __asm__("push bc");
  __asm__("push de");
  __asm__("push hl");
  __asm__("ex af, af'");
  __asm__("exx");

  __asm__("call _GTSTCK");

  __asm__("ex af, af'");
  __asm__("exx");
  __asm__("pop hl");
  __asm__("pop de");
  __asm__("pop bc");
  __asm__("pop af");
  __asm__("ex af, af'");
  __asm__("exx");

  __asm__("pop  iy");
  __asm__("pop  ix");
  __asm__("pop  hl");
  __asm__("pop  de");
  __asm__("pop  bc");
  __asm__("ret");
}

#else

uint8_t msx_GTSTCK(const uint8_t a) __naked {
  ((void)a);                    // (sp+2) --> A
  __asm__("push af");
  __asm__("push bc");
  __asm__("push de");
  __asm__("push hl");
  __asm__("push ix");
  __asm__("push iy");

  __asm__("ld ix, #12");
  __asm__("add ix, sp");
  __asm__("ld a, 2 (ix)");

  __asm__("ex af, af'");
  __asm__("exx");
  __asm__("push af");
  __asm__("push bc");
  __asm__("push de");
  __asm__("push hl");
  __asm__("ex af, af'");
  __asm__("exx");

  __asm__("call _GTSTCK");

  __asm__("ex af, af'");
  __asm__("exx");
  __asm__("pop hl");
  __asm__("pop de");
  __asm__("pop bc");
  __asm__("pop af");
  __asm__("ex af, af'");
  __asm__("exx");

  __asm__("pop iy");
  __asm__("pop ix");
  __asm__("pop hl");
  __asm__("ld l, a");
  __asm__("pop de");
  __asm__("pop bc");
  __asm__("pop af");

  __asm__("ret");
}

#endif
