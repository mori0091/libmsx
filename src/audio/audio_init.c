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
 * \file audio_init.c
 */

#include "audio__internal.h"

#include <audio_buf.h>
#include <bios.h>

void audio_init(void) {
  audio_.vsync_freq = msx_get_vsync_frequency();
  if (SCC_find(&audio_.scc)) {
    // SCC_set_mode(&audio_.scc, 2);
    SCC_enable(&audio_.scc);
  }
  if (OPLL_find(&audio_.opll)) {
    OPLL_enable(&audio_.opll);
  }
  audio_buf_init();
}
