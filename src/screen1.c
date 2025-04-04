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
 * \file screen1.c
 */

#include "screen.h"

#include "tty.h"
#include <stdint.h>
#include <vdp.h>
#include <vmem.h>
#include <sprite.h>

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

#define COLUMNS (32)
#define LINES   (24)
#define POS     (COLUMNS * CSRY + CSRX)

static void clear_screen(void) {
  vmem_memset(IMAGE, ' ', SIZE_OF_IMAGE);
}

static void render_char(uint8_t c) {
  vmem_set_write_address(IMAGE + POS);
  vmem_set(c);
}

static void set_text_color(uint8_t fg, uint8_t bg) {
  /* Set foreground and background color */
  vmem_memset(COLORS, ((fg & 15) << 4) | (bg & 15), SIZE_OF_COLORS);
}
static void set_border_color(uint8_t border) {
  /* Set backdrop color (border color of the screen) */
  vdp_set_color(border & 15);
}

static const struct TTY_Device dev = {
  .columns = COLUMNS,
  .lines = LINES,
  .clear_screen = clear_screen,
  .render_char = render_char,
  .set_text_color = set_text_color,
  .set_border_color = set_border_color,
};

void screen1(void) {
  if (1 < msx_get_version()) {
    vdp_set_yjk_mode(VDP_RGB);
  }
  if (msx_get_version()) {
    vdp_set_screen_lines(VDP_SCREEN_LINES_192);
  }

  /* zero clear 16KiB VRAM */
  vmem_memset(0, 0, 16384);

  vdp_set_screen_mode(SCREEN_MODE);

  vdp_set_image_table(IMAGE);
  vdp_set_pattern_table(PATTERNS);
  vdp_set_color_table(COLORS);

  vdp_set_sprite_attribute_table(SPRITES);
  vdp_set_sprite_pattern_table(SPRITE_PATTERNS);
  vmem_init_sprites(SPRITES);

  // Copy MSX fonts into VRAM
  vmem_write(PATTERNS, (void *)CGTBL, 8*256);

  TTY_device = &dev;
  TTY_set_color(FORCLR, BAKCLR, BDRCLR);
  TTY_cls();
}
