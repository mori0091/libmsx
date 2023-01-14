// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2023 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file bdos.c
 */

#include <config.h>
#include "bdos.h"

#ifndef __SDCCCALL
#define __sdcccall(x)
#endif

void bdos(uint8_t c, struct z80_reg * reg) __sdcccall(0) __naked {
  (void)c;                      // (sp+2)  --> C
  (void)reg;                    // (sp+3) --> IX
  __asm__("push af");
  __asm__("push bc");
  __asm__("push de");
  __asm__("push hl");
  __asm__("push ix");
  __asm__("push iy");

  __asm__("ld ix, #12");
  __asm__("add ix, sp");
  __asm__("ld c, 2 (ix)");
  __asm__("ld e, 3 (ix)");
  __asm__("ld d, 4 (ix)");
  __asm__("push de");
  __asm__("pop ix");

  __asm__("ld a, 1 (ix)");
  __asm__("ld b, 3 (ix)");
  __asm__("ld e, 4 (ix)");
  __asm__("ld d, 5 (ix)");
  __asm__("ld l, 6 (ix)");
  __asm__("ld h, 7 (ix)");
  __asm__("push ix");
  __asm__("call 0xf37d");
  __asm__("pop ix");
  __asm__("ld 1 (ix), a");
  __asm__("ld 2 (ix), c");
  __asm__("ld 3 (ix), b");
  __asm__("ld 4 (ix), e");
  __asm__("ld 5 (ix), d");
  __asm__("ld 6 (ix), l");
  __asm__("ld 7 (ix), h");

  __asm__("pop iy");
  __asm__("pop ix");
  __asm__("pop hl");
  __asm__("pop de");
  __asm__("pop bc");
  __asm__("pop af");

  __asm__("ret");
}
