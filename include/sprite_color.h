// -*- coding: utf-8-unix -*-

#ifndef SPRITE_COLOR_H
#define SPRITE_COLOR_H

#pragma once

#include <stdint.h>

#include "sprite.h"
#include "vmem.h"

// element of sprite color table (for sprite mode 2)
struct sprite_color {
  tagged_color_t tagged_colors[16]; // EC | CC | IC | 0 | Color Code
};

/**
 * Set sprite color table element (tagged color for each scanlines) of a sprite.
 *
 * - `MSX2`, `MSX2+`, `MSXturboR`
 * - `SPRITE MODE 2`
 *
 * \param base  Base address of the SPRITE ATTRIBUTE TABLE in VRAM.
 * \param plane The plane number.
 * \param sprite_color
 *              Pointer to constructed sprite color table element,
 *              that to be copied into VRAM.
 */
void vmem_set_sprite_color(vmemptr_t base, uint8_t plane,
                           const struct sprite_color* sprite_color);

/**
 * Set sprite color (tagged color) of a sprite.
 *
 * - `MSX`, `MSX2`, `MSX2+`, `MSXturboR`
 * - `SPRITE MODE 1`, `SPRITE MODE 2`
 *
 * \param base  Base address of the SPRITE ATTRIBUTE TABLE in VRAM.
 * \param plane The plane number.
 * \param tagged_color
 *              A color code and sprite attribute tags, that
 *              is applied for all sprites in the metasprite.
 */
void vmem_set_sprite_color_s(vmemptr_t base, uint8_t plane,
                             const tagged_color_t tagged_color);

/**
 * Set sprite color (tagged color) of some sprites.
 *
 * - `MSX`, `MSX2`, `MSX2+`, `MSXturboR`
 * - `SPRITE MODE 1`, `SPRITE MODE 2`
 *
 * \param base  Base address of the SPRITE ATTRIBUTE TABLE in VRAM.
 * \param plane The plane number.
 * \param tagged_colors
 *              Array of color code and sprite attribute tags, that
 *              is applied for each sprite of the sprite plane number
 *              from `plane` to `plane + n -1`, one by one.
 */
void vmem_set_sprite_color_m(vmemptr_t base, uint8_t plane, uint8_t n,
                             const tagged_color_t* tagged_colors);

#endif
