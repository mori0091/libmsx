// -*- coding: utf-8-unix -*-
/**
 * \file msx_GTTRIG.c
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

uint8_t msx_GTTRIG(const uint8_t a) __naked {
  ((void)a);                    // A --> A
  __asm__("jp _GTTRIG");
}

#else

uint8_t msx_GTTRIG(const uint8_t a) __naked {
  ((void)a);                    // (sp+2) --> A
  __asm__("push af");
  __asm__("push hl");
  __asm__("ld   hl, #6");
  __asm__("add  hl, sp");
  __asm__("ld   a, (hl)");
  __asm__("call _GTTRIG");
  __asm__("pop hl");
  __asm__("ld   l, a");
  __asm__("pop af");
  __asm__("ret");
}

#endif
