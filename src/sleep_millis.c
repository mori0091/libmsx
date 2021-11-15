// -*- coding: utf-8-unix -*-
/**
 * \file sleep_millis.c
 *
 * Copyright (c) 2021 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

#include "../include/bios.h"
#include "../include/interrupt.h"

void sleep_millis(uint16_t ms) {
  // sleep_ticks(VSYNC_FREQ * ms / 1000); // bad!
  // when (VSYNC_FREQ * ms) > 65535, the computation overflows.
  // the below avoids that situation.
  const uint8_t VSYNC_FREQ = msx_get_vsync_frequency();
  sleep_ticks(VSYNC_FREQ * (ms / 1000) +
              VSYNC_FREQ * (ms % 1000) / 1000);
}
