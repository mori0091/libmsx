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
 * \file bmem_get_u16.c
 */

#include "bmem.h"

uint16_t bmem_get_u16(bmemptr_t src) {
  return bmem_get(src) | ((uint16_t)bmem_get(src + 1) << 8);
}
