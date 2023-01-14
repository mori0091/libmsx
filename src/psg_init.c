// -*- coding: utf-8-unix -*-
/**
 * \file psg_init.c
 *
 * Copyright (c) 2021-2023 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

#include "../include/psg.h"

const uint8_t psg_reg_initial_vector[14] = {
  0x55, 0x00,                   // R#0-1 channel A frequency division ratio
  0x00, 0x00,                   // R#2-3 channel B frequency division ratio
  0x00, 0x00,                   // R#4-5 channel C frequency division ratio
  0x00,                         // R#6 noise frequency division ratio
  0xb8,                         // R#7 mixer
  0x00,                         // R#8 volume A
  0x00,                         // R#9 volume B
  0x00,                         // R#10 volume C
  0x0b, 0x00,                   // R#11-12 envelope cycle
  0x00,                         // R#13 envelope pattern
};

void psg_init(void) {
  __asm__("di");
  for (uint8_t i = 0; i < sizeof(psg_reg_initial_vector); ++i) {
    psg_set(i, psg_reg_initial_vector[i]);
  }
  __asm__("ei");
}
