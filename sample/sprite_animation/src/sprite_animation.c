// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2025 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file sprite_animation.c
 */

#include "interrupt.h"
#include "sm2.h"
#include "vdp.h"
#include "workarea.h"
#include <msx.h>
#include <resources.h>
#include <screen.h>
#include <stdint.h>
#include <text.h>

static void description(void) {
  locate(0, 3);
  text_color(13, 10);
  print("ase2msx (https://github.com/mori0091/ase2msx)\n\n");
  text_color(15, 0);
  print(">ase2msx A.ase\n");
  text_color(0, 10);
  print("\n");
  print(" Creates sprite data for libmsx/MSX2 from Aseprite file 'A.ase'\n");
  print(" (See also https://www.aseprite.org/ about Aseprite)\n");
  print("\n");
  print(" Aseprite's meta-data   : A.json\n");
  print(" Aseprite's sprite sheet: A.png\n");
  text_color(13, 10);
  print(" C header for libmsx    : A.h\n");
  print(" C source for libmsx    : A.c\n");
  print(" spritte data for libmsx: A.sm2\n");
  text_color(0, 10);
  print("   - pattern table\n");
  print("   - color table (template)\n");
  print("   - attribute table (template)\n");
  print("\n");
  print("The sprite must be\n");
  print(" Multiple of 16x16 pixels\n");
  print(" IndexColor w/ up to 16 colors\n");
  print(" color code #0 is transparent\n");
}

const static palette_t palette[] = {
  [ 0] = RGB(0,0,0), // [  0   0   0]
  [ 1] = RGB(1,1,1), // [ 43  44  74]
  [ 2] = RGB(3,2,1), // [107  73  37]
  [ 3] = RGB(6,3,1), // [184  88  42]
  [ 4] = RGB(6,6,6), // [188 184 182]
  [ 5] = RGB(7,5,3), // [210 140  97]
  [ 6] = RGB(7,0,4), // [255   0 122]
  [ 7] = RGB(2,6,7),
  [ 8] = RGB(7,1,1),
  [ 9] = RGB(7,3,3),
  [10] = RGB(6,6,1),
  [11] = RGB(6,6,4),
  [12] = RGB(1,4,1),
  [13] = RGB(6,2,5),
  [14] = RGB(5,5,5),
  [15] = RGB(7,7,7),
};

static void show_palette(void) {
  for (uint8_t i = 0; i < 16; ++i) {
    locate(i, 25);
    text_color((i+1)&15, i);
    printx(i);
  }
}

// include generated header file.
#include "TJ_dog.h"

static sm2_Sprite sprite;

void perform_animation_step(void) {
  // Calculate the remaining time of the current animation frame and update the
  // frame number.
  sm2_update_sprites(&sprite, 1);
}

void main(void) {
  screen7();
  color(15, 10, 0);
  cls();

  description();

  // The color palette must be initialized manually.
  // (The palette data is not included in the sprite sheet data file.)
  vdp_write_palette(palette);
  show_palette();

  // VDP register setting for sprites
  // - Sprite size (16x16 or 16x16_MAGNIFIED)
  vdp_set_sprite_size(VDP_SPRITE_SIZE_16x16_MAGNIFIED);

  // Initialize SM2 module.
  sm2_init();

  // Initialize sprite sheet.
  TJ_dog_init();

  // Construct/Initialize animated sprite.
  sm2_init_sprite(&sprite, &TJ_dog, &TJ_dog_tag_Walk);

  // Perform an animation step at each VSYNC timing.
  set_vsync_handler(perform_animation_step);

  // Main loop
  for (;;) {
    // Add some sprites to the internal list.
    // In this case, the same sprite is added twice with different positions.
    sm2_add_sprite(&sprite, 160, 64);
    sm2_add_sprite(&sprite, 128, 128);
    // Next, the list is flushed to update the displayed sprites.
    sm2_flush();
  } // Repeat the above as needed and/or periodically.
}
