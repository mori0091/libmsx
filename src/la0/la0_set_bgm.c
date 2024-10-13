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
 * \file la0_set_bgm.c
 */

#include "la0__internal.h"

#include <audio_cb.h>

void la0_set_bgm(uint8_t idx, MemFile * mf) {
  if (la0__open(idx, mf, &la0_.bgm)) {
    if (la0_.bgm.soundchip_enable & 2) {
      // SCC+ mode
      audio_cb_scc_mode(2);
    }
    else if (la0_.bgm.soundchip_enable & 1) {
      // SCC compatible mode
      audio_cb_scc_mode(1);
    }
    audio_cb_bgm_changed(la0_.bgm.rate, &LA0_BGM_DECODER);
  }
  __asm__("ei");
}
