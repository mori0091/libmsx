// -*- coding: utf-8-unix -*-
/**
 * \file boot_main.c
 *
 * Copyright (c) 2021-2023 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

#pragma codeseg BANK2

#include "boot_main.h"
#include "macros.h"

const palette_t color_palette[16] = {
  RGB(0,0,0), RGB(6,1,0), RGB(3,3,1), RGB(7,5,4),
  RGB(4,6,0), RGB(0,5,0), RGB(1,5,7), RGB(7,4,1),
  RGB(6,2,0), RGB(1,1,1), RGB(7,5,5), RGB(6,2,0),
  RGB(2,4,7), RGB(6,1,0), RGB(7,7,7), RGB(7,5,0),
};

void boot_init_vdp(void);
void boot_init_vmem(void);

void boot_main(void) __banked {
  CLIKSW = 0;
  boot_init_vdp();
  boot_init_vmem();
}

void boot_init_vdp(void) {
  /* Set backdrop color (border color of the screen) */
  vdp_set_color(0x00);

  vdp_set_visible(false);
  vdp_set_screen_mode(SCREEN_MODE);
  vdp_set_screen_lines(SCREEN_LINES);

  // vdp_set_pattern_table(PATTERNS);
  // vdp_set_color_table(COLORS);

  vdp_set_sprite_visible(false);
  vdp_set_sprite_attribute_table(SPRITES);
  vdp_set_sprite_pattern_table(SPRITE_PATTERNS);
  vdp_set_sprite_size(VDP_SPRITE_SIZE_16x16);

  /* Dual page horizontal scroll mode */
  vdp_set_image_table(IMAGE + SIZE_OF_VRAM_PAGE); /* SET PAGE 1 */
  vdp_set_hscroll_dual_page(true); /* enable 2 pages horizontal scroll */
  vdp_set_hscroll_mask(true);      /* hide ugly left border of 8 pixels */
  vdp_set_adjust(-4, 0);           /* centering visible display area */

  /* Apply palette table to VDP */
  vdp_write_palette(color_palette);
}

#define VRAM_PAGE_FILL(pp, byte)                              \
  vdp_cmd_execute_HMMV(0,                                     \
                       (pp)*LINES_PER_VRAM_PAGE,              \
                       PIXELS_PER_LINE,                       \
                       LINES_PER_VRAM_PAGE,                   \
                       (byte))

void boot_init_vmem(void) {
  /* clear VRAM page #0 and #1 */
  VRAM_PAGE_FILL(0, 0xcc);
  VRAM_PAGE_FILL(1, 0xcc);

  // In rare cases, access to VRAM may fail or may be omitted depending on the
  // timing of the access. This occurs during a VDP command is executing.
  // Therefore, waits for VDP command finished before accsess to VRAM.
  vdp_cmd_await();
  /* Hide all sprites */
  vmem_memset(SPRITES, 217, sizeof(struct sprite) * 32);

  /* Draw color palette at top(^_^?) of VRAM page #0 and bottom of VRAM page #1 */
  /* (debug purpose only) */
  for (int i = 0; i < 16; ++i) {
    uint8_t even_odd_color = (i << 4) + i;
    vdp_cmd_execute_HMMV(16 * i + 1,   1 + 0 * LINES_PER_VRAM_PAGE, 14, 14, even_odd_color);
    vdp_cmd_execute_HMMV(16 * i + 0,  16 + 0 * LINES_PER_VRAM_PAGE, 16, 16, even_odd_color);
    vdp_cmd_execute_HMMV(16 * i + 0, 196 + 1 * LINES_PER_VRAM_PAGE, 16, 16, even_odd_color);
  }
}
