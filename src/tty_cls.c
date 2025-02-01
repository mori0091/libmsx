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
 * \file tty_cls.c
 */

#include "../include/tty.h"

void TTY_cls(void) {
  if (TTY_device && TTY_device->clear_screen) {
    TTY_device->clear_screen();
  }
  TTY_locate(0, 0);
}
