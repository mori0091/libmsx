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
 * \file sprite_color.h
 * \brief Sprite color table interface.
 * \ingroup SPRITES
 */

#pragma once

#ifndef SPRITE_COLOR_H
#define SPRITE_COLOR_H

#include <stdint.h>

#include "sprite.h"
#include "vmem.h"

/**
 * \defgroup SPRITES_COLOR Sprite color table
 * \ingroup SPRITES
 * Sprite color table interface.
 * @{
 */

/**
 * `MSX2` Element of sprite color table (for sprite mode 2).
 */
struct sprite_color {
  tagged_color_t tagged_colors[16]; ///< `EC` | `CC` | `IC` | `0` | `Color Code`
};

/**
 * `MSX2` Set sprite color table element (tagged color for each scanlines) of a sprite.
 *
 * \param base  Base address of the SPRITE ATTRIBUTE TABLE in VRAM.
 * \param plane The plane number.
 * \param sprite_color
 *              Pointer to constructed sprite color table element,
 *              that to be copied into VRAM.
 *
 * \note For use with `SPRITE MODE 2`.
 */
void vmem_set_sprite_color(vmemptr_t base, uint8_t plane,
                           const struct sprite_color* sprite_color);

/**
 * `MSX` Set sprite color (tagged color) of a sprite.
 *
 * \param base  Base address of the SPRITE ATTRIBUTE TABLE in VRAM.
 * \param plane The plane number.
 * \param tagged_color
 *              A color code and sprite attribute tags, that is applied for
 *              sprite of the sprite plane number `plane`.
 *
 * \note For use with `SPRITE MODE 1`.
 */
void vmem_set_sprite_color_s(vmemptr_t base, uint8_t plane,
                             const tagged_color_t tagged_color);

/**
 * `MSX` Set sprite color (tagged color) of some sprites.
 *
 * \param base  Base address of the SPRITE ATTRIBUTE TABLE in VRAM.
 * \param plane The plane number.
 * \param n     Number of planes
 * \param tagged_colors
 *              Array of color code and sprite attribute tags, that
 *              is applied for each sprite of the sprite plane number
 *              from `plane` to `plane + n -1`, one by one.
 *
 * \note For use with `SPRITE MODE 1`.
 */
void vmem_set_sprite_color_m(vmemptr_t base, uint8_t plane, uint8_t n,
                             const tagged_color_t* tagged_colors);

/** @} */

#endif
