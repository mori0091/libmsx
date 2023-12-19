// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2023 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file audio_pause.c
 */

#include "audio__internal.h"

#include <ay_3_8910.h>
#include <scc_buf.h>
#include <opll_buf.h>

void audio_pause(void) {
  __asm__("di");
  audio_.paused = true;
  ay_3_8910_stop();
  SCC_stop(&audio_.scc);
  OPLL_stop(&audio_.opll);
  __asm__("ei");
}
