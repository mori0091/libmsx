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
 * \file NDP_start_bmem.c
 */

#include <NDP.h>
#include "./NDP__internal.h"

extern void NDP_MSTART(void);

void NDP_start_bmem(void) {
  NDP_.paused = false;
  if (NDP_is_playing()) return;
  __asm__("push ix");
  __asm__("di");
  if (NDP_.song_bank) {
    const uint8_t bank = bmem_get_bank();
    bmem_set_bank(NDP_.song_bank);
    NDP_MSTART();
    bmem_set_bank(bank);
  }
  else {
    NDP_MSTART();
  }
  __asm__("ei");
  __asm__("pop ix");
}
