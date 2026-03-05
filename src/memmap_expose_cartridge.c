// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2026 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file memmap_expose_cartridge.c
 */

#include "./memmap.h"

void memmap_expose_cartridge(void) {
__asm__("ENASLT = 0x0024");
__asm__("push    ix");
__asm__("ld      a, (_libmsx_initial_slots + #1)");
__asm__("ld      h, #0x40");
__asm__("call    ENASLT");
__asm__("ld      a, (_libmsx_initial_slots + #2)");
__asm__("ld      h, #0x80");
__asm__("call    ENASLT");
__asm__("pop     ix");
}
