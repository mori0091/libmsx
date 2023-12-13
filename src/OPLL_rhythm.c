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
 * \file opll_buf.c
 */

#include <opll_buf.h>

void OPLL_rhythm(uint8_t val) {
  OPLL_put(0x0e, (val & 0x1f) | 0x20);
}
