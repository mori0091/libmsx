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
 * \file stdio_putchar.c
 * \brief `putchar()` - A standard C library function.
 */

#include <stdio.h>

#include "tty.h"

int putchar(int c) {
  TTY_put((uint8_t)c);
  return c;
}
