// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2025 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */
/**
 * \file psg_set.c
 */

#include "../include/psg.h"

void psg_set(uint8_t reg, uint8_t val) {
  psg_port0 = reg;
  psg_port1 = val;
}
