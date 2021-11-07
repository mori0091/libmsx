// -*- coding: utf-8-unix -*-
/**
 * \file psg_set.c
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

void psg_set(uint8_t reg, uint8_t val) {
  __critical {
    psg_port0 = reg;
    psg_port1 = val;
  }
}
