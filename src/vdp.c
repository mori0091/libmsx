// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2025 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file vdp.c
 */

#include "../include/vdp.h"

#include "vdp_internal.h"

volatile enum vdp_screen_mode screen_mode = VDP_SCREEN_MODE_GRAPHIC_1;
volatile uint8_t sprite_mode = 1;
