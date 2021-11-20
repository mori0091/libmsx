// -*- coding: utf-8-unix -*-
/**
 * \file vdp_internal.h
 *
 * Copyright (c) 2021 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

#ifndef VDP_INTERNAL_H
#define VDP_INTERNAL_H

#pragma once

#include <stdint.h>

extern volatile enum vdp_screen_mode screen_mode;
extern volatile uint8_t sprite_mode;

#define VDP_CMD_LMMV (0x80)
#define VDP_CMD_LMMM (0x90)
#define VDP_CMD_HMMV (0xc0)
#define VDP_CMD_HMMM (0xd0)

#endif
