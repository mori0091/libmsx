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
 * \file tty.c
 */

#include "tty.h"

#include <stdint.h>
#include <vmem.h>
#include <workarea.h>

const struct TTY_Device * TTY_device;

#define COLUMNS  (TTY_device->columns)
#define LINES    (TTY_device->lines)

static void TTY_check_pos(void) {
  if (!TTY_device) return;
  while (COLUMNS <= CSRX) {
    CSRX -= COLUMNS;
    CSRY++;
  }
  while (LINES <= CSRY) {
    CSRY -= LINES;              // or scroll..
  }
}

void TTY_locate(uint8_t x, uint8_t y) {
  CSRX = x;
  CSRY = y;
  TTY_check_pos();
}

void TTY_linefeed(void) {
  CSRX = 0;
  CSRY++;
  TTY_check_pos();
}

void TTY_cls(void) {
  if (TTY_device && TTY_device->clear_screen) {
    TTY_device->clear_screen();
  }
  TTY_locate(0, 0);
}

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

void TTY_set_fg_color(uint8_t fg) {
  TTY_set_text_color(fg, BAKCLR);
}

void TTY_set_bg_color(uint8_t bg) {
  TTY_set_text_color(FORCLR, bg);
}

void TTY_set_border_color(uint8_t border) {
  BDRCLR = border;
  if (TTY_device && TTY_device->set_border_color) {
    TTY_device->set_border_color(border);
  }
}

void TTY_set_text_color(uint8_t fg, uint8_t bg) {
  FORCLR = fg;
  BAKCLR = bg;
  if (TTY_device && TTY_device->set_text_color) {
    TTY_device->set_text_color(fg, bg);
  }
}

void TTY_set_color(uint8_t fg, uint8_t bg, uint8_t border) {
  TTY_set_text_color(fg, bg);
  TTY_set_border_color(border);
}
