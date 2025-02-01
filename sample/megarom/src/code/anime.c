// -*- coding: utf-8-unix -*-
/**
 * \file anime.c
 *
 * Copyright (c) 2021-2025 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

#include "anime.h"

/**
 * Tick counter (TIME) of MSX-BASIC
 */
static volatile __at (0xfc9e) uint16_t JIFFY;

/* colors for palette animation of coin / ?-block */
static const palette_t coin_colors[] = {
  RGB(7,4,1), RGB(6,2,0), RGB(3,0,0), RGB(6,2,0),
  RGB(7,4,1), RGB(7,4,1), RGB(7,4,1), RGB(7,4,1),
};

inline void coin_palette_animate(void) {
  /* palette animation (coin / ?-block) */
  if (!(JIFFY & 7)) {
    const uint8_t j = (JIFFY >> 3) & 7;
    vdp_set_palette(7, coin_colors[j]);
  }
}

void anime_on_vsync(void) {
  coin_palette_animate();
}
