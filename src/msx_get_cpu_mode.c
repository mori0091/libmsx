// -*- coding: utf-8-unix -*-
/**
 * \file msx_get_cpu_mode.c
 *
 * Copyright (c) 2021-2023 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */

#include "../include/bios.h"

uint8_t msx_get_cpu_mode(void) {
  if (msx_get_version() < 3) return 0;
  return msx_GETCPU();
}
