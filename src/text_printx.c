// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2025 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file text_printx.c
 */

#include "text.h"

#include "tty.h"

static void putxdigit(uint8_t c) {
  if (9 < c) {
    TTY_put('a' + c - 10);
  }
  else {
    TTY_put('0' + c);
  }
}

void printx(uint16_t x) {
  if (15 < x) {
    printx(x / 16);
  }
  putxdigit(x & 15);
}
