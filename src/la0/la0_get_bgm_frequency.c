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
 * \file la0_get_bgm_frequency.c
 */

#include "la0__internal.h"

uint8_t la0_get_bgm_frequency(void) {
  return la0_.bgm.rate;
}
