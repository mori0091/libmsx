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
 * \file tty_set_text_color.c
 */

#include "../include/tty.h"

#include "../include/workarea.h"

void TTY_set_text_color(uint8_t fg, uint8_t bg) {
  FORCLR = fg;
  BAKCLR = bg;
  if (TTY_device && TTY_device->set_text_color) {
    TTY_device->set_text_color(fg, bg);
  }
}
