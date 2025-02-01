// -*- coding: utf-8-unix -*-
/**
 * \file sleep_ticks.c
 *
 * Copyright (c) 2021-2025 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

#include "../include/interrupt.h"

void sleep_ticks(uint16_t ticks) {
  while (ticks--) {
    await_vsync();
  }
}
