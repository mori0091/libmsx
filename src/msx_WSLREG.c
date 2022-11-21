// -*- coding: utf-8-unix -*-
/**
 * \file msx_WSLREG.c
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

void msx_WSLREG(uint8_t value) __naked {
  (void)value;                  // A --> A
  __asm__("jp _WSLREG");
}

#else

void msx_WSLREG(uint8_t value) __naked {
  (void)value;                  // (sp+2) --> A
  __asm__("push af");
  __asm__("push ix");
  __asm__("ld ix, #4");
  __asm__("add ix, sp");
  __asm__("ld a, 2 (ix)");
  __asm__("pop ix");
  __asm__("call _WSLREG");
  __asm__("pop af");
  __asm__("ret");
}

#endif
