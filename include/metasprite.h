// -*- coding: utf-8-unix -*-

#ifndef METASPRITE_H
#define METASPRITE_H

#pragma once

#include <stdint.h>

#include "vdp.h"
#include "vmem.h"
#include "sprite.h"

typedef struct vec2i {
  int x;
  int y;
} vec2i_t;

typedef struct metasprite {
  /** number of sprites of that the metasprite consists. */
  uint8_t n;
  /** anchor point in metasprite local coordinates */
  vec2i_t origin;
  /** `n` element array of sprite positions */
  vec2i_t* layouts;
  /** `n` element array of sprite pattern numbers */
  uint8_t* pats;
} metasprite_t;

/**
 * Display single colored metasprite.
 *
 * \param base  Base address of the sprite attribute table in VRAM.
 * \param plane The plane number on which the 1st sprite of the metasprite is
 *              displayed.
 * \param x     The x-coordinate in the screen coordinate system.
 * \param y     The y-coordinate in the screen coordinate system.
 * \param ms    Pointer to the metasprite structure.
 * \param tagged_color
 *              A color code and sprite attribute tags, that
 *              is applied for all sprites in the metasprite.
 */
void vmem_set_metasprite_s(vmemptr_t base, uint8_t plane, int x, int y,
                           const metasprite_t* ms,
                           const tagged_color_t tagged_color);

/**
 * Display multi colored metasprite.
 *
 * \param base  Base address of the sprite attribute table in VRAM.
 * \param plane The plane number on which the 1st sprite of the metasprite is
 *              displayed.
 * \param x     The x-coordinate in the screen coordinate system.
 * \param y     The y-coordinate in the screen coordinate system.
 * \param ms    Pointer to the metasprite structure.
 * \param tagged_colors
 *              Array of color code and sprite attribute tags, that
 *              is applied for each sprite in the metasprite, one by one.
 */
void vmem_set_metasprite_m(vmemptr_t base, uint8_t plane, int x, int y,
                           const metasprite_t* ms,
                           const tagged_color_t* tagged_colors);

/**
 * Display / Move metasprite.
 *
 * \note
 * This function does not change the sprite's color, tag bits, or the contents
 * of the sprite color table. It only updates the x, y, and sprite pattern
 * number in the sprite attribute table.
 * - Pros: Works a little faster in sprite mode 2.
 * - Cons: The EC (Early Clock) bit is not set correctly.
 *
 * This function is supposed to be used together with vmem_set_sprite_color() in
 * sprite mode 2.
 *
 * In sprite mode 1 (MSX1 compatible sprite mode), you may want to use
 * vmem_set_metasprite_s() or vmem_set_metasprite_m(). These are fast enough and
 * convenient.
 *
 * In sprite mode 2, vmem_set_metasprite_s() and vmem_set_metasprite_m() are
 * also useful. However, they are a bit slower because they need to copy the
 * color and tag bits to the color table element of each sprite.
 *
 * \param base  Base address of the sprite attribute table in VRAM.
 * \param plane The plane number on which the 1st sprite of the metasprite is
 *              displayed.
 * \param x     The x-coordinate in the screen coordinate system.
 * \param y     The y-coordinate in the screen coordinate system.
 * \param ms    Pointer to the metasprite structure.
 */
void vmem_set_metasprite_a(vmemptr_t base, uint8_t plane, int x, int y,
                           const metasprite_t* ms);

#endif
