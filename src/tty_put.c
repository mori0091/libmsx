// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2024 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file tty_put.c
 */

#include "../include/tty.h"

#include "../include/workarea.h"

extern void TTY_check_pos(void);

static void TTY_put_char(uint8_t c) {
  if (TTY_device && TTY_device->render_char) {
    TTY_device->render_char(c);
  }
  CSRX++;
  TTY_check_pos();
}

void TTY_put(uint8_t c) {
  // \todo implement TAB, BS, etc.
  if (c == '\n') {
    TTY_linefeed();
  }
  else {
    TTY_put_char(c);
  }
}
