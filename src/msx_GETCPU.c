// -*- coding: utf-8-unix -*-
/**
 * \file msx_CHGCPU.c
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

uint8_t msx_GETCPU(void) __naked {
  __asm__("jp _GETCPU");
}

#else

uint8_t msx_GETCPU(void) __naked {
  __asm__("push af");
  __asm__("call _GETCPU");
  __asm__("ld l, a");
  __asm__("pop af");
  __asm__("ret");
}

#endif
