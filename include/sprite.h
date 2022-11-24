// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file sprite.h
 * \brief Sprites.
 */

#pragma once

#ifndef SPRITE_H
#define SPRITE_H

#include <stdint.h>

#include "vmem.h"

// ---- Sprite

/**
 * Enumerations for tags of sprite attribute table.
 */
enum sprite_tag {
  SPRITE_TAG_IC = (1<<5),       // IC bit (Ignore Collision)
  SPRITE_TAG_CC = (1<<6),       // CC bit (Compose Color / Cancel Collision)
  SPRITE_TAG_EC = (1<<7),       // EC bit (Early Clock)
};

/**
 * Color code with tags of sprite attribute table.
 */
typedef uint8_t tagged_color_t;

/**
 * Element of sprite attribute table.
 */
struct sprite {
  uint8_t y;                    ///< y coordinates
  uint8_t x;                    ///< x coordinates
  uint8_t pat;                  ///< sprite pattern number
  tagged_color_t tagged_color;  ///< `EC` | `CC` | `IC` | `0` | `Color Code`
};

/**
 * `MSX` Set x coordinates to a `struct sprite`.
 * \param s  pointer to a `struct sprite`.
 * \param x  x value.
 */
void sprite_set_x(struct sprite* s, int x);
/**
 * `MSX` Set y coordinates to a `struct sprite`.
 * \param s  pointer to a `struct sprite`.
 * \param y  y value.
 */
void sprite_set_y(struct sprite* s, int y);
/**
 * `MSX` Set x and y coordinates to a `struct sprite`.
 * \param s  pointer to a `struct sprite`.
 * \param x  x value.
 * \param y  y value.
 */
void sprite_set_xy(struct sprite* s, int x, int y);
/**
 * `MSX` Set sprite pattern number to a `struct sprite`.
 * \param s    pointer to a `struct sprite`.
 * \param pat  sprite pattern number.
 */
void sprite_set_pat(struct sprite* s, uint8_t pat);
/**
 * `MSX` Set tags and color code to a `struct sprite`.
 * \param s             pointer to a `struct sprite`.
 * \param tagged_color  tags and color code.
 */
void sprite_set_color(struct sprite* s, tagged_color_t tagged_color);

/**
 * `MSX` Set sprite attribute table element.
 *
 * \param base  Base address of the SPRITE ATTRIBUTE TABLE in VRAM.
 * \param plane The plane number.
 * \param s     Pointer to constructed sprite attribute table element,
 *              that to be copied into VRAM.
 */
inline
void vmem_set_sprite(vmemptr_t base, uint8_t plane, const struct sprite* s) {
  vmem_write(base + plane * sizeof(struct sprite), (void*)s, sizeof(struct sprite));
}

#endif
