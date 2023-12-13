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
 * \file opll_buf.c
 */

#include <opll_buf.h>

void OPLL_rhythm_mode(bool on) {
  if (on) {
    OPLL_put(0x0e, 0x20);
    OPLL_put(0x16, 0x20);
    OPLL_put(0x17, 0x50);
    OPLL_put(0x18, 0xC0);
    OPLL_put(0x26, 0x05);
    OPLL_put(0x27, 0x05);
    OPLL_put(0x28, 0x01);
    OPLL_put(0x36, 0x00);
    OPLL_put(0x37, 0x00);
    OPLL_put(0x38, 0x00);
  }
  else {
    OPLL_put(0x0e, 0x00);
  }
}
