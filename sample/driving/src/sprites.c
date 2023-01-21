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
 * \file sprites.c
 * \brief SPRITE part of `MSX2+` driving game demo.
 */

#include <msx.h>

#include "./sprites.h"

#define SPRITES         (0x07600) // sprite attribute table
#define SPRITE_PATTERNS (0x07800) // sprite pattern generator table

const uint8_t sprites[][32] = {
  [0] = {                       /* << */
    0b01111111,
    0b10000000,
    0b10000001,
    0b10000011,
    0b10000111,
    0b10001111,
    0b10011111,
    0b10111111,

    0b10011111,
    0b10001111,
    0b10000111,
    0b10000011,
    0b10000001,
    0b10000000,
    0b01111111,
    0b00000000,

    0b11111110,
    0b00000001,
    0b11111101,
    0b11111001,
    0b11110001,
    0b11100001,
    0b11000001,
    0b10000001,

    0b11000001,
    0b11100001,
    0b11110001,
    0b11111001,
    0b11111101,
    0b00000001,
    0b11111110,
    0b00000000,
  },
  [1] = {                       /* >> */
    0b01111111,
    0b10000000,
    0b10111111,
    0b10011111,
    0b10001111,
    0b10000111,
    0b10000011,
    0b10000001,

    0b10000011,
    0b10000111,
    0b10001111,
    0b10011111,
    0b10111111,
    0b10000000,
    0b01111111,
    0b00000000,

    0b11111110,
    0b00000001,
    0b10000001,
    0b11000001,
    0b11100001,
    0b11110001,
    0b11111001,
    0b11111101,

    0b11111001,
    0b11110001,
    0b11100001,
    0b11000001,
    0b10000001,
    0b00000001,
    0b11111110,
    0b00000000,
  },
  [2] = {                       /* ■ */
    0b01111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,

    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b01111111,
    0b00000000,

    0b11111110,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,

    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111110,
    0b00000000,
  },
};

const vec2i_t layouts[] = { { 0, 0, }, { 0, 0, }, };
const uint8_t curve_indicator_L_pats[] = { 0, 8, };
const uint8_t curve_indicator_R_pats[] = { 4, 8, };
const metasprite_t curve_indicators[] = {
  [0] = { .n = 2, .layouts = (vec2i_t *)layouts, .pats = (uint8_t *)&curve_indicator_L_pats[0], },
  [1] = { .n = 2, .layouts = (vec2i_t *)layouts, .pats = (uint8_t *)&curve_indicator_R_pats[0], },
};
const tagged_color_t curve_indicators_color[] = { 8, 10, };

void setup_sprites(void) {
  vdp_set_sprite_size(VDP_SPRITE_SIZE_16x16_MAGNIFIED);
  vmem_write(SPRITE_PATTERNS, (void *)sprites, sizeof(sprites));
  vmem_set_sprite_color_m(SPRITES, 0, 2, curve_indicators_color);
}

void hide_curve_indicator(void) {
  vmem_set_metasprite_a(SPRITES, 0, 0, 217, &curve_indicators[0]);
}

void show_curve_indicator_right(void) {
  vmem_set_metasprite_a(SPRITES, 0, 160, 64, &curve_indicators[1]);
}

void show_curve_indicator_left(void) {
  vmem_set_metasprite_a(SPRITES, 0, 64, 64, &curve_indicators[0]);
}
