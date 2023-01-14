// -*- coding: utf-8-unix -*-
/**
 * \file msx_CHGCPU.c
 *
 * Copyright (c) 2021-2023 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

#include "../include/bios.h"

#if (__SDCCCALL == 1)

void msx_CHGCPU(const uint8_t mode) __naked {
  ((void)mode);
  __asm__("push af");
  __asm__("and a, #0x83");
  __asm__("call _CHGCPU");
  __asm__("pop af");
  __asm__("ret");
}

#else

void msx_CHGCPU(const uint8_t mode) __naked {
  ((void)mode);
  __asm__("push af");
  __asm__("push hl");

  __asm__("ld hl, #6");
  __asm__("add hl, sp");
  __asm__("ld a, (hl)");
  __asm__("and a, #0x83");
  __asm__("call _CHGCPU");

  __asm__("pop hl");
  __asm__("pop af");
  __asm__("ret");
}

#endif
