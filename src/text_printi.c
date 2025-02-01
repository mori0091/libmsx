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
 * \file text_printi.c
 */

#include "text.h"

#include "tty.h"

void printi(int i) {
  if (i < 0) {
    TTY_put('-');
    i = -i;
  }
  printu(i);
}
