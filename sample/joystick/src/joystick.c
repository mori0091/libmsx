// -*- coding: utf-8-unix -*-
/**
 * \file joystick.c
 * Sample : get and show joystick button state.
 *
 * Copyright (c) 2021 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */

#include <stdint.h>
#include <stdlib.h>
#include <msx.h>

/* Configurations for GRAPHIC 1 mode (SCREEN 1) */
#define SCREEN_MODE     VDP_SCREEN_MODE_GRAPHIC_1
#define PATTERNS        (0x00000) // pattern generator table
#define IMAGE           (0x01800) // pattern name table
#define COLORS          (0x02000) // color table
#define SPRITES         (0x01B00) // sprite attribute table
#define SPRITE_PATTERNS (0x03800) // sprite pattern generator table
#define SIZE_OF_PATTERNS (0x0800) // size of pattern generator table
#define SIZE_OF_IMAGE    (0x0300) // size of pattern name table
#define SIZE_OF_COLORS   (32)     // size of color table

static void init_vdp(void) {
  /* Set backdrop color (border color of the screen) */
  vdp_set_color(0x07);

  vdp_set_screen_mode(SCREEN_MODE);

  vdp_set_image_table(IMAGE);
  vdp_set_pattern_table(PATTERNS);
  vdp_set_color_table(COLORS);

  vdp_set_sprite_attribute_table(SPRITES);
  vdp_set_sprite_pattern_table(SPRITE_PATTERNS);
  vdp_set_sprite_size(VDP_SPRITE_SIZE_16x16);
}

static void clear_vram(void) {
  /* clear screen */
  vmem_memset(IMAGE, ' ', SIZE_OF_IMAGE);
  /* foreground color = 15, background color = 4 */
  vmem_memset(COLORS, 0x0f4, SIZE_OF_COLORS);
}

static uint8_t text_pos_x = 0;
static uint8_t text_pos_y = 0;

static void locate(uint8_t x, uint8_t y) {
  if (32 <= x) x = 31;
  if (24 <= y) y = 23;
  text_pos_x = x;
  text_pos_y = y;
}

static void putc(int c) {
  if (c == '\n') {
    text_pos_x = 0;
    text_pos_y++;
  }
  else {
    vmem_write(IMAGE + text_pos_x + text_pos_y * 32,
               (uint8_t*)&c, 1);
    text_pos_x++;
  }
  if (32 <= text_pos_x) {
    text_pos_x = 0;
    text_pos_y++;
  }
  if (24 <= text_pos_y) {
    text_pos_y = 0;
  }
}

static void puts(const char* str) {
  while (*str) {
    putc(*str++);
  }
}

static void puts_joystate(const uint8_t state, const char* str) {
  for (int i = 7; i >= 0; --i) {
    putc('0' + ((state >> i) & 1));
  }
  puts(str);
}

static void print_joypad_state(void) {
  locate(0,0);
  puts_joystate(joypad_get_state(0), " : SPACE and ARROW keys\n");
  puts_joystate(joypad_get_state(1), " : joystick #1 (port A)\n");
  puts_joystate(joypad_get_state(2), " : joystick #2 (port B)\n");
  puts("  ||||||\n"
       "  |||||`-- VK_UP\n"
       "  ||||`--- VK_DOWN\n"
       "  |||`---- VK_LEFT\n"
       "  ||`----- VK_RIGHT\n"
       "  |`------ VK_FIRE_0\n"
       "  `------- VK_FIRE_1\n");
}

void main(void) {
  init_vdp();
  clear_vram();

  for (;;) {
    await_vsync();
    print_joypad_state();
  }
}
