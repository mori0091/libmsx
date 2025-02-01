// -*- coding: utf-8-unix -*-
/**
 * \file macros.h
 *
 * Copyright (c) 2021-2025 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

#pragma once

#ifndef MACROS_H_
#define MACROS_H_

#include <msx.h>

#define max(a, b)               ((a) >= (b) ? (a) : (b))

// ---- configurations of GRAPHIC 4 (SCREEN 5)
#define SCREEN_MODE             VDP_SCREEN_MODE_GRAPHIC_4
#define SCREEN_LINES            VDP_SCREEN_LINES_212
#define IMAGE                   (0x00000)
#define SPRITE_PATTERNS         (0x07800)
#define SPRITES                 (0x07600)
#define SPRITE_COLORS           (SPRITES - 0x0200)

#define PIXELS_PER_BYTE         (2)
#define PIXELS_PER_LINE         (256)
#define LINES_PER_VRAM_PAGE     (256)

#define SIZE_OF_VRAM_PAGE       ((uint32_t)0x08000)
#define SIZE_OF_IMAGE           (0x06A00)
#define SIZE_OF_SPRITE_PATTERNS (256 * 8)
#define SIZE_OF_SPRITE_COLORS   (32 * sizeof(struct sprite_color))
#define SIZE_OF_SPRITES         (32 * sizeof(struct sprite))

#endif  // MACROS_H_
