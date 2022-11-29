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
 * \file screen6.c
 */

#include "screen.h"

#include "tty.h"
#include <stdint.h>
#include <vdp.h>
#include <vmem.h>
#include <sprite.h>

/* Configurations for GRAPHIC 5 mode (SCREEN 6) */
#define SCREEN_MODE     VDP_SCREEN_MODE_GRAPHIC_5
#define IMAGE           (0x00000) // pattern name table
#define SPRITES         (0x07600) // sprite attribute table
#define SPRITE_PATTERNS (0x07800) // sprite pattern generator table
#define SIZE_OF_IMAGE    (0x6A00) // size of pattern name table

#define WIDTH           (512)             // number of pixels per line
#define HEIGHT          (212)             // number of lines
#define BPP             (2)               // nuber of bits per pixel
#define BYTES_PER_LINE  (WIDTH * BPP / 8) // number of bytes per line

#define COLUMNS (64)
#define LINES   (26)
#define POS     ((vmemptr_t)BYTES_PER_LINE * 8 * CSRY + BPP * CSRX)

static void clear_screen(void) {
  uint8_t c = (BAKCLR & 3) * 0x55;
  vdp_cmd_execute_HMMV(0, 0, WIDTH, HEIGHT, c);
}

static void render_char(uint8_t c) {
  vmemptr_t dst = IMAGE + POS;
  const uint8_t * p = &CGTBL->data[c][0];
  for (uint8_t n = 8; n--; ) {
    vmem_set_write_address(dst);
    dst += BYTES_PER_LINE;
    uint8_t bits = *p++;
    for (uint8_t i = 2; i--; ) {
      uint8_t c
        = ((((bits & 0x80) ? FORCLR : BAKCLR) & 3) << 6)
        | ((((bits & 0x40) ? FORCLR : BAKCLR) & 3) << 4)
        | ((((bits & 0x20) ? FORCLR : BAKCLR) & 3) << 2)
        | ((((bits & 0x10) ? FORCLR : BAKCLR) & 3) << 0);
      vmem_set(c);
      bits <<= 4;
    }
  }
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

void screen6(void) {
  vdp_set_screen_lines(VDP_SCREEN_LINES_212);

  vdp_set_screen_mode(SCREEN_MODE);

  vdp_set_image_table(IMAGE);

  vdp_set_sprite_attribute_table(SPRITES);
  vdp_set_sprite_pattern_table(SPRITE_PATTERNS);

  vmem_init_sprites(SPRITES);

  TTY_device = &dev;
  TTY_set_color(FORCLR, BAKCLR, BDRCLR);
  TTY_cls();
}
