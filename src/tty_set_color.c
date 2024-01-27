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
 * \file tty_set_color.c
 */

#include "../include/tty.h"

void TTY_set_color(uint8_t fg, uint8_t bg, uint8_t border) {
  TTY_set_text_color(fg, bg);
  TTY_set_border_color(border);
}
