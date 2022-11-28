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
 * \file text_print.c
 */

#include "text.h"

#include "tty.h"

void print(const char* str) {
  while (*str) {
    TTY_put((uint8_t)*str++);
  }
}
