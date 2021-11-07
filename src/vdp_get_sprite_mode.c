// -*- coding: utf-8-unix -*-
/**
 * \file vdp_get_sprite_mode.c
 *
 * Copyright (c) 2021 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

#include "../include/vdp.h"

#include "vdp_internal.h"

uint8_t vdp_get_sprite_mode(void) {
  return sprite_mode;
}
