// -*- coding: utf-8-unix -*-
/**
 * \file psg_get.c
 *
 * Copyright (c) 2021 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

#include "../include/io.h"
#include "../include/psg.h"

uint8_t psg_get(uint8_t reg) {
  uint8_t val;
  __critical {
    psg_port0 = reg;
    val = psg_port2;
  }
  return val;
}
