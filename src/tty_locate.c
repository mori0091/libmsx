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
 * \file tty_locate.c
 */

#include "../include/tty.h"

#include "../include/workarea.h"

extern void TTY_check_pos(void);

void TTY_locate(uint8_t x, uint8_t y) {
  CSRX = x;
  CSRY = y;
  TTY_check_pos();
}
