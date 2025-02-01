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
 * \file im_adtr.c
 */

#include "im_adtr.h"

#define ADTR(bit)                                     \
  uint8_t im_adtr ## bit (const uint8_t * src) {      \
    uint8_t a = 0;                                    \
    for (uint8_t i = 8; i--; ) {                      \
      a >>= 1;                                        \
      if (*src++ & (1 << bit)) {                      \
        a |= 0x80;                                    \
      }                                               \
    }                                                 \
    return a;                                         \
  }

ADTR(0)
ADTR(1)
ADTR(2)
ADTR(3)
ADTR(4)
ADTR(5)
ADTR(6)
ADTR(7)
