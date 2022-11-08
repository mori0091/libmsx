// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2022 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file volume_gauge.c
 */

#include <vmem.h>
#include "./screen1.h"

const char volume_gauge[17][5] = {
  [ 0] = { 32,  32,  32,  32, 0},
  [ 1] = {128,  32,  32,  32, 0},
  [ 2] = {129,  32,  32,  32, 0},
  [ 3] = {130,  32,  32,  32, 0},
  [ 4] = {131,  32,  32,  32, 0},
  [ 5] = {131, 128,  32,  32, 0},
  [ 6] = {131, 129,  32,  32, 0},
  [ 7] = {131, 130,  32,  32, 0},
  [ 8] = {131, 131,  32,  32, 0},
  [ 9] = {131, 131, 128,  32, 0},
  [10] = {131, 131, 129,  32, 0},
  [11] = {131, 131, 130,  32, 0},
  [12] = {131, 131, 131,  32, 0},
  [13] = {131, 131, 131, 128, 0},
  [14] = {131, 131, 131, 129, 0},
  [15] = {131, 131, 131, 130, 0},
  [16] = {132, 132, 132, 132, 0}, // Saw wave pattern
};

const uint8_t saw_pat[8] = {
  0x40, 0x40, 0x50, 0x50, 0x54, 0x54, 0x55, 0x00,
};

void init_volume_gauge_chars(void) {
  vmem_memset(PATTERNS+128*8, 0, 4*8);
  vmem_memset(PATTERNS+128*8, 0x40, 7); // '|   '
  vmem_memset(PATTERNS+129*8, 0x50, 7); // '||  '
  vmem_memset(PATTERNS+130*8, 0x54, 7); // '||| '
  vmem_memset(PATTERNS+131*8, 0x55, 7); // '||||'
  vmem_write(PATTERNS+132*8, (void *)saw_pat, 8);
}
