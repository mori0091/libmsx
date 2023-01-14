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
 * \file text_printu.c
 */

#include "text.h"

#include "tty.h"

void printu(unsigned int i) {
  if (9 < i) {
    printu(i / 10);
  }
  TTY_put('0' + (i % 10));
}
