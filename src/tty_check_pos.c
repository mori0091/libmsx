// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2022 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file tty_check_pos.c
 */

#include "../include/tty.h"

#include "../include/workarea.h"

#define COLUMNS  (TTY_device->columns)
#define LINES    (TTY_device->lines)

void TTY_check_pos(void) {
  if (!TTY_device) return;
  while (COLUMNS <= CSRX) {
    CSRX -= COLUMNS;
    CSRY++;
  }
  while (LINES <= CSRY) {
    CSRY -= LINES;              // or scroll..
  }
}
