// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2024 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file tile.c
 */

#include "./tile.h"

BM8x8 tileset[256];

// Read into the tileset from the 256-character pattern in the main ROM.
void copy_tileset_from_MainROM(void) {
  memcpy(tileset, (void *)CGTBL, sizeof(*CGTBL));
}

// Write the tileset to the pattern generator table in VRAM.
void copy_tileset_to_VRAM(void) {
  vmem_write(PATTERNS, tileset, sizeof(tileset));
}

// Pre-defined sprite attribute table.
const struct sprite sprites[] = {
{ .y =  71, .x =  40, .pat =  0, .tagged_color = 15 },
{ .y =  71, .x =  88, .pat =  4, .tagged_color =  2 },
{ .y =  71, .x = 136, .pat =  8, .tagged_color = 11 },
{ .y =  71, .x = 184, .pat = 12, .tagged_color =  9 },

{ .y = 111, .x =  40, .pat = 64, .tagged_color =  7 },
{ .y = 111, .x =  88, .pat = 68, .tagged_color =  3 },
{ .y = 111, .x = 136, .pat = 48, .tagged_color = 13 },
{ .y = 111, .x = 184, .pat = 56, .tagged_color =  6 },

{ .y = 151, .x =  40, .pat = 32, .tagged_color = 14 },
{ .y = 151, .x =  88, .pat = 36, .tagged_color = 12 },
{ .y = 151, .x = 136, .pat = 40, .tagged_color = 10 },
{ .y = 151, .x = 184, .pat = 44, .tagged_color =  8 },

// If y = 208(218) in sprite mode 1(2),
// all subsequent sprites are hidden.
{ .y = 208, },
};

static void vmem_fill(uint8_t val, size_t nbytes) {
  while (nbytes--) {
    vmem_set(val);
  }
}

void main(void) {
  screen2();
  cls();

  // Initialize the pattern name table.
  vmem_set_write_address(IMAGE);
  for (int i = 0; i < 256; ++i) {
    vmem_set(i);
  }
  // Initialize the color table.
  vmem_set_write_address(COLORS);
  for (uint8_t i = 4; i--; ) {
    for (uint8_t j = 16; j--; ) {
      vmem_fill(0xf4, 8);
      vmem_fill(0xf5, 8);
    }
    for (uint8_t j = 16; j--; ) {
      vmem_fill(0xf5, 8);
      vmem_fill(0xf4, 8);
    }
  }
  // Colorize some character patterns with the colors of the corresponding
  // sprites.
  for (uint8_t i = 0; i < 12; ++i) {
    uint8_t fg = sprites[i].tagged_color << 4;
    uint8_t c = sprites[i].pat;
    vmem_set_write_address(COLORS + 8 * c);
    if (c & 32) {
      vmem_fill(fg | 5, 8);
      vmem_fill(fg | 4, 8);
      vmem_fill(fg | 5, 8);
      vmem_fill(fg | 4, 8);
    }
    else {
      vmem_fill(fg | 4, 8);
      vmem_fill(fg | 5, 8);
      vmem_fill(fg | 4, 8);
      vmem_fill(fg | 5, 8);
    }
  }

  // Initialize sprites.
  vdp_set_sprite_size(VDP_SPRITE_SIZE_16x16_MAGNIFIED);
  // - Use the pattern generator table as also the sprite pattern table.
  vdp_set_sprite_pattern_table(PATTERNS);
  // - Show sprites.
  vmem_write(SPRITES, (void *)sprites, sizeof(sprites));

  for (;;) {
    // [Demo] Matrix transformation and complement operators for 8x8 bitmap patterns.
    demo_8x8_unary_operators();
    // [Demo] Boolean operators for 8x8 bitmap patterns.
    demo_8x8_binary_operators();
    // [Demo] Shift / Rotate (Scroll) operators for 8x8 bitmap patterns.
    demo_8x8_shift_operators();

    // [Demo] Matrix transformation and complement operators for 16x16 sprite patterns.
    demo_16x16_unary_operators();
    // [Demo] Boolean operators for 16x16 sprite patterns.
    demo_16x16_binary_operators();
    // [Demo] Shift / Rotate (Scroll) operators for 16x16 sprite patterns.
    demo_16x16_shift_operators();

    // [Demo] Shift / Scroll row vector of 8x8 bitmap patterns.
    demo_8x8_vec_shift_operators();
    // [Demo] Crop (Trim) 16x16 Sprite patterns.
    demo_16x16_trimming_operator();
    // [Demo] Tile mapping.
    demo_8x8_tilemap_operator();
  }
}
