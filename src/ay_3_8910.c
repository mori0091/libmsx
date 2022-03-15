// -*- coding: utf-8-unix -*-
/**
 * \file ay_3_8910.c
 *
 * Copyright (c) 2022 Daishi Mori (mori0091)
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
  ay_3_8910_buffer[13] = 0xff;
  ay_3_8910_play();
}

void ay_3_8910_play(void) {
  __critical {
    {
      uint8_t x = ay_3_8910_buffer[7];
      x &= 0x3f;
      x |= 0x80;
      ay_3_8910_buffer[7] = x;
    }
    uint8_t * p = ay_3_8910_buffer;
    uint8_t reg = 0;
    /* R#0..R#10 */
    while (reg < 11) {
      psg_set(reg++, *p++);
    }
    /* R#11..R#13 */
    if (ay_3_8910_buffer[13] < 16) {
      psg_set(reg++, *p++);
      psg_set(reg++, *p++);
      psg_set(reg++, *p++);
    }
    ay_3_8910_buffer[13] = 0xff;
  }
}
