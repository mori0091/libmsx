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

#include "./sprites.h"

#include <msx.h>

#include <stdint.h>
#include <string.h>

#define SPRITE_COLORS   (SPRITES - 512) // sprite color table
#define SPRITES         (0x07600) // sprite attribute table
#define SPRITE_PATTERNS (0x07800) // sprite pattern generator table

static struct sprite_table {
  struct sprite_color color[32]; // sprite color table
  struct sprite attr[32];        // sprite attribute table
} sprites;

const uint8_t sprite_patterns[][32] = {
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

  // ________ ________ ________ ________
  // ________ ________ ________ ________
  // ________ ________ ________ ________
  // ________ ________ ________ ________
  // ________ ________ ________ ________
  // ________ ________ ________ ________
  // ________ ________ ________ ________
  // ________ ________ ________ ________
  // ________ _******* *******_ ________
  // _______* ******** ******** *_______
  // ______** ******** ******** **______
  // ______** ******** ******** **______
  // _____*** ******** ******** ***_____
  // _____*** ******** ******** ***_____
  // ____**** ******** ******** ****____
  // ____**__ ________ ________ __**____

  // ___***** ******** ******** *****___
  // ___***** ******** ******** *****___
  // __****** ******** ******** ******__
  // __****** ******** ******** ******__
  // _******* ******** ******** *******_
  // _******* ******** ******** *******_
  // _******* ******** ******** *******_
  // _******* ******** ******** *******_
  // _******* ******** ******** *******_
  // __****** ******** ******** ******__
  // __****** ******** ******** ******__
  // ___***** ******** ******** *****___
  // ____**** ***_____ _____*** ****____
  // ____**** ***_____ _____*** ****____
  // ________ ________ ________ ________
  // ________ ________ ________ ________

  [3] = {                       /* vehicle(0,0) */
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000001,
    0b00000011,
    0b00000011,
    0b00000111,
    0b00000111,
    0b00001111,
    0b00001100,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b01111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b00000000,
  },

  [4] = {                       /* vehicle(1,0) */
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b11111110,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b10000000,
    0b11000000,
    0b11000000,
    0b11100000,
    0b11100000,
    0b11110000,
    0b00110000,
  },

  [5] = {                       /* vehicle(0,1) */
    0b00011111,
    0b00011111,
    0b00111111,
    0b00111111,
    0b01111111,
    0b01111111,
    0b01111111,
    0b01111111,
    0b01111111,
    0b00111111,
    0b00111111,
    0b00011111,
    0b00001111,
    0b00001111,
    0b00000000,
    0b00000000,

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
    0b11100000,
    0b11100000,
    0b00000000,
    0b00000000,
  },

  [6] = {                       /* vehicle(1,1) */
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
    0b00000111,
    0b00000111,
    0b00000000,
    0b00000000,

    0b11111000,
    0b11111000,
    0b11111100,
    0b11111100,
    0b11111110,
    0b11111110,
    0b11111110,
    0b11111110,
    0b11111110,
    0b11111100,
    0b11111100,
    0b11111000,
    0b11110000,
    0b11110000,
    0b00000000,
    0b00000000,
  },
};

const tagged_color_t curve_indicators_colors[] = { 8, 10, };
const vec2i_t curve_indicator_layouts[] = { {0}, {0}, };
const uint8_t curve_indicator_L_pats[] = { 0, 8, };
const uint8_t curve_indicator_R_pats[] = { 4, 8, };
const metasprite_t curve_indicators[] = {
  [0] = { .n = 2, .layouts = (vec2i_t *)curve_indicator_layouts, .pats = (uint8_t *)&curve_indicator_L_pats[0], },
  [1] = { .n = 2, .layouts = (vec2i_t *)curve_indicator_layouts, .pats = (uint8_t *)&curve_indicator_R_pats[0], },
};

const tagged_color_t vehicle_colors[] = { 1, 1, 1, 1 };
const vec2i_t vehicle_layouts[] = {
  { 0, 0}, {32, 0},
  { 0,32}, {32,32},
};
const vec2i_t vehicle_L_layouts[] = {
  {-2, 0}, {30,-1},
  { 0,32}, {32,31},
};
const vec2i_t vehicle_R_layouts[] = {
  { 2,-1}, {34, 0},
  { 0,31}, {32,32},
};
const uint8_t vehicle_pats[] = {
  12, 16,
  20, 24,
};
const metasprite_t vehicle = {
  .n = 4, .layouts = (vec2i_t *)vehicle_layouts, .pats = (uint8_t *)vehicle_pats,
};
const metasprite_t vehicle_L = {
  .n = 4, .layouts = (vec2i_t *)vehicle_L_layouts, .pats = (uint8_t *)vehicle_pats,
};
const metasprite_t vehicle_R = {
  .n = 4, .layouts = (vec2i_t *)vehicle_R_layouts, .pats = (uint8_t *)vehicle_pats,
};

void update_sprite_colors(void) {
  vmem_write(SPRITE_COLORS, (void *)&sprites.color, sizeof(sprites.color));
}

void update_sprite_attributes(void) {
  vmem_write(SPRITES, (void *)&sprites.attr, sizeof(sprites.attr));
}

void update_sprite_color(uint8_t plane) {
  // assert(plane < 32);
  vmem_write(SPRITE_COLORS + sizeof(struct sprite_color) * plane,
             (void *)&sprites.color[plane],
             sizeof(struct sprite_color));
}

void update_sprite_attribute(uint8_t plane) {
  // assert(plane < 32);
  vmem_write(SPRITES + sizeof(struct sprite) * plane,
             (void *)&sprites.attr[plane],
             sizeof(struct sprite));
}

static void set_sprite_color(uint8_t plane, tagged_color_t color) {
  // assert(plane < 32);
  memset(&sprites.color[plane], color, sizeof(struct sprite_color));
  update_sprite_color(plane);
}

static void set_sprite_colors(uint8_t plane, uint8_t n, const tagged_color_t * colors) {
  // assert(plane < 32 && plane + n <= 32);
  while (n--) {
    set_sprite_color(plane++, *colors++);
  }
}

static void set_sprite_raster_color(uint8_t plane, const struct sprite_color * color) {
  // assert(plane < 32);
  memcpy(&sprites.color[plane], color, sizeof(struct sprite_color));
  update_sprite_color(plane);
}

static void set_sprite_raster_colors(uint8_t plane, uint8_t n, const struct sprite_color * colors) {
  // assert(plane < 32 && plane + n <= 32);
  while (n--) {
    set_sprite_raster_color(plane++, colors++);
  }
}

static void set_metasprite(uint8_t plane, const metasprite_t * ms, int x, int y) {
  // assert(plane < 32 && plane + ms->n <= 32);
  struct sprite * sp = &sprites.attr[plane];
  const uint8_t n = ms->n;
  const int dx = x - ms->anchor.x;
  const int dy = y - ms->anchor.y;
  for (uint8_t i = 0; i < n; ++i) {
    sp->pat = ms->pats[i];
    sprite_set_x(sp, ms->layouts[i].x + dx);
    sprite_set_y(sp, ms->layouts[i].y + dy);
    if (sp->tagged_color & SPRITE_TAG_EC) {
      // if there is a sprite whose x-coordinate would to be less than 0 on the
      // screen, that sprite is hidden instead of setting the EC bit.
      sp->y = 217;
    }
    sp++;
  }
}

static void unset_metasprite(uint8_t plane, const metasprite_t * ms) {
  // assert(plane < 32 && plane + ms->n <= 32);
  struct sprite * sp = &sprites.attr[plane];
  for (uint8_t i = ms->n; i--; ) {
    sp->y = 217;
    sp++;
  }
}

static void show_metasprite(uint8_t plane, const metasprite_t * ms, int x, int y) {
  // assert(plane < 32 && plane + ms->n <= 32);
  set_metasprite(plane, ms, x, y);
  vmem_write(SPRITES + sizeof(struct sprite) * plane,
             &sprites.attr[plane],
             sizeof(struct sprite) * ms->n);
}

static void hide_metasprite(uint8_t plane, const metasprite_t * ms) {
  // assert(plane < 32 && plane + ms->n <= 32);
  unset_metasprite(plane, ms);
  vmem_write(SPRITES + sizeof(struct sprite) * plane,
             &sprites.attr[plane],
             sizeof(struct sprite) * ms->n);
}

void setup_sprites(void) {
  vdp_set_sprite_size(VDP_SPRITE_SIZE_16x16_MAGNIFIED);
  vmem_write(SPRITE_PATTERNS, (void *)sprite_patterns, sizeof(sprite_patterns));
  // vmem_set_sprite_color_m(SPRITES, 0, 2, curve_indicators_colors);
  set_sprite_colors(0, 2, curve_indicators_colors);
  // vmem_set_sprite_color_m(SPRITES, 2, 4, vehicle_colors);
  set_sprite_colors(2, 4, vehicle_colors);
}

void hide_curve_indicator(void) {
  // vmem_set_metasprite_a(SPRITES, 0, 0, 217, &curve_indicators[0]);
  hide_metasprite(0, &curve_indicators[0]);
}

void show_curve_indicator_right(void) {
  // vmem_set_metasprite_a(SPRITES, 0, 160, 64, &curve_indicators[1]);
  show_metasprite(0, &curve_indicators[1], 160, 64);
}

void show_curve_indicator_left(void) {
  // vmem_set_metasprite_a(SPRITES, 0, 64, 64, &curve_indicators[0]);
  show_metasprite(0, &curve_indicators[0], 64, 64);
}

void hide_vehicle(void) {
  hide_metasprite(2, &vehicle);
}

void show_vehicle(void) {
  show_metasprite(2, &vehicle, 96, 140);
}

void show_vehicle_L(void) {
  show_metasprite(2, &vehicle_L, 96, 140);
}

void show_vehicle_R(void) {
  show_metasprite(2, &vehicle_R, 96, 140);
}
