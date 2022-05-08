// -*- coding: utf-8-unix -*-
/**
 * \file sprite.h
 *
 * Copyright (c) 2021 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */

#pragma once

#ifndef SPRITE_H
#define SPRITE_H

#include <stdint.h>

#include "vmem.h"

// ---- Sprite

enum sprite_tag {
  SPRITE_TAG_IC = (1<<5),       // IC bit (Ignore Collision)
  SPRITE_TAG_CC = (1<<6),       // CC bit (Compose Color / Cancel Collision)
  SPRITE_TAG_EC = (1<<7),       // EC bit (Early Clock)
};

typedef uint8_t tagged_color_t;

// element of sprite attribute table
struct sprite {
  uint8_t y;
  uint8_t x;
  uint8_t pat;                  // sprite pattern number
  union {
    tagged_color_t tagged_color; // EC | CC | IC | 0 | Color Code
    uint8_t unused_in_sprite_mode_2;
  };
};

void sprite_set_x(struct sprite* s, int x);
void sprite_set_y(struct sprite* s, int y);
void sprite_set_xy(struct sprite* s, int x, int y);
void sprite_set_pat(struct sprite* s, uint8_t pat);
void sprite_set_color(struct sprite* s, tagged_color_t tagged_color);

/**
 * Set sprite attribute table element.
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
