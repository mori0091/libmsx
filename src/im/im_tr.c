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
 * \file im_tr.c
 */

#include "im_tr.h"

#define TR(bit)                                       \
  uint8_t im_tr ## bit (const uint8_t * src) {        \
    uint8_t a = 0;                                    \
    for (uint8_t i = 8; i--; ) {                      \
      a <<= 1;                                        \
      if (*src++ & (1 << bit)) {                      \
        a |= 1;                                       \
      }                                               \
    }                                                 \
    return a;                                         \
  }

TR(0)
TR(1)
TR(2)
TR(3)
TR(4)
TR(5)
TR(6)
TR(7)
