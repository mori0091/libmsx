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
 * \file sm2_get_reserved.c
 */

#include <sm2.h>
#include "./sm2_internal.h"

uint8_t sm2_get_reserved(void) {
  return sm2__num_planes_reserved;
}
