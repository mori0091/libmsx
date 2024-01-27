// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2024 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */
/**
 * \file psg_get.c
 */

#include "../include/psg.h"

uint8_t psg_get(uint8_t reg) {
  psg_port0 = reg;
  return psg_port2;
}
