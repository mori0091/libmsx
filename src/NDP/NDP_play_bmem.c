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
 * \file NDP_play_bmem.c
 */

#include <NDP.h>
#include "./NDP__internal.h"

void NDP_play_bmem(void) {
  if (NDP_.paused) return;
  if (NDP_.song_bank) {
    // Play the song in MegaROM
    const uint8_t bank = bmem_get_bank();
    bmem_set_bank(NDP_.song_bank);
    NDP_INTRPT();
    bmem_set_bank(bank);
  }
  else {
    NDP_INTRPT();
  }
}
