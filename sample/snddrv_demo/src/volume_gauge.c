// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2023 Daishi Mori (mori0091)
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

#define PATTERNS        (0x00000) // pattern generator table

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

static const uint8_t pats[] = {
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, // '|   '
  0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, // '||  '
  0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54, // '||| '
  0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, // '||||'
  0x40, 0x40, 0x50, 0x50, 0x54, 0x54, 0x55, 0x00, // Saw tooth
};

void init_volume_gauge_chars(void) {
  vmem_write(PATTERNS+128*8, (void *)pats, sizeof(pats));
}
