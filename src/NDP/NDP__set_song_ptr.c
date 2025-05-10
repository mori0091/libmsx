// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2025 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file NDP__set_song_ptr.c
 */

#include "NDP__internal.h"

void NDP__set_song_ptr(uint8_t bank, void * ptr) {
  (void)bank;                   // A
  (void)ptr;                    // DE
  __asm__("di");
  __asm__("ld (_NDP_+0), a");   // NDP_.song_bank = bank;
  __asm__("call _NDP_ADRSET");
  __asm__("ei");
}
