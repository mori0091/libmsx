// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2024 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file la0_set_sfx.c
 */

#include "la0__internal.h"

#include <audio_cb.h>

void la0_set_sfx(uint8_t idx, MemFile * mf) {
  if (la0__open(idx, mf, &la0_.sfx)) {
    la0_.sfx.priority = 0;
    audio_cb_sfx_changed(la0_.sfx.rate, &LA0_SFX_DECODER);
    // if (la0_.sfx.soundchip_enable & 2) {
    //   // SCC+ mode
    //   audio_cb_scc_mode(2);
    // }
    // else if (la0_.sfx.soundchip_enable & 1) {
    //   // SCC compatible mode
    //   audio_cb_scc_mode(1);
    // }
  }
  __asm__("ei");
}
