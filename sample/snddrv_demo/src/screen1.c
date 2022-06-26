// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021 Daishi Mori (mori0091)
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

#include <vdp.h>
#include <vmem.h>

#include "./screen1.h"

void screen1(void) {
  init_vdp();
  clear_vmem();
}

void init_vdp(void) {
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

void clear_vmem(void) {
  /* clear screen */
  vmem_memset(IMAGE, ' ', SIZE_OF_IMAGE);
  /* foreground color = 15, background color = 4 */
  vmem_memset(COLORS, 0x0f4, SIZE_OF_COLORS);
}

static uint8_t text_pos_x = 0;
static uint8_t text_pos_y = 0;

void locate(uint8_t x, uint8_t y) {
  if (32 <= x) x = 31;
  if (24 <= y) y = 23;
  text_pos_x = x;
  text_pos_y = y;
}

void putc(int c) {
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

void puts(const char* str) {
  while (*str) {
    putc(*str++);
  }
}

static char buf[5+1] = {0};
void puti(int i) {
  if (!i) {
    putc('0');
    return;
  }
  if (i < 0) {
    putc('-');
    i = -i;
  }
  char * p = buf + sizeof(buf) - 1;
  do {
    --p;
    *p = '0' + i % 10;
    i /= 10;
  } while (i && p > buf);
  puts(p);
}
