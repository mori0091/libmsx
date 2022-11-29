// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2022 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file screen4.c
 */

#include "screen.h"

#include "tty.h"
#include <stdint.h>
#include <vdp.h>
#include <vmem.h>

/* Configurations for GRAPHIC 3 mode (SCREEN 4) */
#define SCREEN_MODE     VDP_SCREEN_MODE_GRAPHIC_3
#define PATTERNS        (0x00000) // pattern generator table
#define IMAGE           (0x01800) // pattern name table
#define COLORS          (0x02000) // color table
#define SPRITES         (0x01E00) // sprite attribute table
#define SPRITE_PATTERNS (0x03800) // sprite pattern generator table
#define SIZE_OF_PATTERNS (0x1800) // size of pattern generator table
#define SIZE_OF_IMAGE    (0x0300) // size of pattern name table
#define SIZE_OF_COLORS   (0x1800) // size of color table

#define COLUMNS (32)
#define LINES   (24)
#define POS     ((COLUMNS * CSRY + CSRX) * 8)

#define TEXT_COLOR       (((FORCLR & 15) << 4) | (BAKCLR & 15))

static void clear_screen(void) {
  vmem_memset(PATTERNS, 0, SIZE_OF_PATTERNS);
  vmem_memset(COLORS, TEXT_COLOR, SIZE_OF_COLORS);
}

static void render_char(uint8_t c) {
  vmem_write(PATTERNS + POS, (void *)&CGTBL->data[c][0], 8);
  vmem_memset(COLORS + POS, TEXT_COLOR, 8);
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
  .set_text_color = 0,
  .set_border_color = set_border_color,
};

void screen4(void) {
  vdp_set_screen_lines(VDP_SCREEN_LINES_192);

  /* zero clear 16KiB VRAM */
  vmem_memset(0, 0, 16384);

  vdp_set_screen_mode(SCREEN_MODE);

  vdp_set_image_table(IMAGE);
  vdp_set_pattern_table(PATTERNS);
  vdp_set_color_table(COLORS);

  vdp_set_sprite_attribute_table(SPRITES);
  vdp_set_sprite_pattern_table(SPRITE_PATTERNS);

  vmem_set_write_address(IMAGE);
  for (int c = 0; c < 768; c++) {
    vmem_set(c & 255);
  }

  TTY_device = &dev;
  TTY_set_color(FORCLR, BAKCLR, BDRCLR);
  TTY_cls();
}
