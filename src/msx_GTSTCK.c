// -*- coding: utf-8-unix -*-
/**
 * \file msx_GTSTCK.c
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

#if (__SDCCCALL == 1)

uint8_t msx_GTSTCK(const uint8_t a) __naked {
  ((void)a);
  __asm__("push ix");           // save IX register (frame pointer)
  __asm__("call _GTSTCK");
  __asm__("pop  ix");           // restore IX register
  __asm__("ret");
}

#else

uint8_t msx_GTSTCK(const uint8_t a) __naked {
  ((void)a);
  __asm__("ld   hl, #2");
  __asm__("add  hl, sp");
  __asm__("ld   a, (hl)");
  __asm__("push ix");           // save IX register (frame pointer)
  __asm__("call _GTSTCK");
  __asm__("ld   l, a");
  __asm__("pop  ix");           // restore IX register
  __asm__("ret");
}

#endif
