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
 * \file color_sprite.c
 */

#include "interrupt.h"
#include "vdp.h"
#include <msx.h>
#include <resources.h>
#include <screen.h>
#include <stdint.h>
#include <text.h>

static void description(void) {
  locate(0, 9);
  text_color(15, 0);
  print(">python3 png2msxsprite.py A.png\n");
  text_color(0, 4);
  print("\n");
  print(" creates sprite data from A.png\n");
  print("\n");
  print(" pattern table  : A_spt.bin\n");
  print(" color table    : A_sct.bin\n");
  print(" attribute table: A_sat.bin\n");
  print("  (2x magnified): A_sat2x.bin\n");
  print("\n");
  print("The PNG must be\n");
  print(" Multiple of 16x16 pixels\n");
  print(" IndexColor w/ up to 16 colors\n");
  print(" color code #0 is transparent\n");
}

/* Configurations for GRAPHIC 4 mode (SCREEN 5) */
#define SCREEN_MODE     VDP_SCREEN_MODE_GRAPHIC_4
#define IMAGE           (0x00000)       // pattern name table
#define SPRITE_COLORS   (SPRITES - 512) // sprite color table
#define SPRITES         (0x07600)       // sprite attribute table
#define SPRITE_PATTERNS (0x07800)       // sprite pattern generator table
#define SIZE_OF_IMAGE   (0x06A00)       // size of pattern name table

const static palette_t palette[] = {
  [ 0] = RGB(0,0,0),            // BLACK(TRANSPARENT)
  [ 1] = RGB(5,1,1),            // DARK_RED
  [ 2] = RGB(7,1,1),            // RED
  [ 3] = RGB(1,4,1),            // GREEN
  [ 4] = RGB(7,5,1),            // ORANGE
  [ 5] = RGB(1,1,5),            // BLUE
  [ 6] = RGB(2,6,7),            // CYAN
  [ 7] = RGB(7,7,7),            // WHITE
  [ 8] = RGB(0,0,0),            // BLACK
  [ 9] = RGB(7,3,3),
  [10] = RGB(6,6,1),
  [11] = RGB(6,6,4),
  [12] = RGB(1,4,1),
  [13] = RGB(6,2,5),
  [14] = RGB(5,5,5),
  [15] = RGB(7,7,7),
};

void main(void) {
  screen5();
  color(15, 4, 0);
  cls();

  description();

  vdp_write_palette(palette);
  for (uint8_t i = 0; i < 16; ++i) {
    locate(i, 24);
    text_color((i+1)&15, i);
    printx(i);
  }

  resource_copy_to_vmem("car_32x32_spt.bin", SPRITE_PATTERNS);
  resource_copy_to_vmem("car_32x32_sct.bin", SPRITE_COLORS);

  for(;;) {
    await_vsync();
    vdp_set_sprite_size(VDP_SPRITE_SIZE_16x16);
    resource_copy_to_vmem("car_32x32_sat.bin", SPRITES);
    sleep_millis(5000);

    await_vsync();
    vdp_set_sprite_size(VDP_SPRITE_SIZE_16x16_MAGNIFIED);
    resource_copy_to_vmem("car_32x32_sat2x.bin", SPRITES);
    sleep_millis(5000);
  }
}
