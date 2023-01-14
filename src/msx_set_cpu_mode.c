// -*- coding: utf-8-unix -*-
/**
 * \file msx_set_cpu_mode.c
 *
 * Copyright (c) 2021-2023 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

#include "../include/bios.h"

void msx_set_cpu_mode(const uint8_t mode) {
  if (msx_get_version() < 3) return;
  msx_CHGCPU(mode);
}
