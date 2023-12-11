// -*- coding: utf-8-unix -*-
/**
 * \file ay_3_8910.c
 *
 * Copyright (c) 2021-2023 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

#include "../include/ay_3_8910.h"
#include <string.h>

uint8_t ay_3_8910_buffer[14];

void ay_3_8910_init(void) {
  memset(ay_3_8910_buffer, 0, sizeof(ay_3_8910_buffer));
  ay_3_8910_buffer[7] = 0xb8;
  ay_3_8910_buffer[13] = 0x80;
  ay_3_8910_play();
}

#define psg_set(r, x) \
  psg_port0 = r;      \
  psg_port1 = x

void ay_3_8910_stop(void) {
  psg_set( 8, 0);
  psg_set( 9, 0);
  psg_set(10, 0);
}

void ay_3_8910_play(void) {
  uint8_t * p = &ay_3_8910_buffer[0];
  psg_set( 0, *p++);
  psg_set( 1, *p++);
  psg_set( 2, *p++);
  psg_set( 3, *p++);
  psg_set( 4, *p++);
  psg_set( 5, *p++);
  psg_set( 6, *p++);
  psg_set( 7, (*p++) & 0x3f | 0x80);
  psg_set( 8, *p++);
  psg_set( 9, *p++);
  psg_set(10, *p++);
  psg_set(11, *p++);
  psg_set(12, *p++);
  if (!(*p & 0x80)) {
    psg_set(13, *p);
    *p = 0x80;
  }
}
