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
 * \file scroll2_sc5.c
 * \brief `MSX2` smooth horizontal scroll.
 *
 * `MSX2` (V9938 VDP) has vertical scrolling functionality.
 * But it hasn't horizontal scrolling functionality.
 *
 * So (software) smooth horizontal scrolling functionality is realized usually
 * on GRAPHIC 2 (SCREEN 2) or GRAPHIC 3 (SCREEN 4) mode.
 *
 * But how about on GRAPHIC 4 (SCREEN 5) bitmap graphics mode ?
 */

#include <msx.h>
#include <screen.h>
#include <text.h>

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "./bgm.h"

// -------------
#define SCREEN_MODE     VDP_SCREEN_MODE_GRAPHIC_4
#define IMAGE           (0x00000) // pattern name table
#define SPRITES         (0x07600) // sprite attribute table
#define SPRITE_PATTERNS (0x07800) // sprite pattern generator table
#define SIZE_OF_IMAGE    (0x6A00) // size of pattern name table

#define WIDTH           (256)             // number of pixels per line
#define HEIGHT          (212)             // number of lines
#define BPP             (4)               // nuber of bits per pixel
#define BYTES_PER_LINE  (WIDTH * BPP / 8) // number of bytes per line

// -------------
/** Height of scroll area in pixel. */
#define LINES              (212)

/** number of frames costs to update full screen. */
#define FRAMES_PER_8_DOT   (8)

#define LINES_PER_FRAME    ((LINES + FRAMES_PER_8_DOT - 1) / FRAMES_PER_8_DOT)

static volatile uint8_t scroll_x;   // 0..255
static volatile uint8_t shift_byte; // (scroll_x >> 3)
static volatile uint8_t shift_bit;  // (scroll_x & 7)
static volatile int8_t scroll_x_direction;
static volatile uint8_t frame_counts;

static volatile uint8_t display_page;
static const vmemptr_t name_table[] = {
  IMAGE,
  IMAGE + 0x8000ULL,
};

static void select_display_page(void) {
  display_page = shift_byte & 1;
  vdp_set_image_table(name_table[display_page]);
}

static void update_active_page(void) {
  if (frame_counts) {
    frame_counts--;
    // update incrementally
    uint16_t y = (uint16_t)frame_counts * LINES_PER_FRAME;
    uint16_t y1 = 256 * display_page;
    uint16_t y2 = 256 - y1;
    if (0 < scroll_x_direction) {
      vdp_cmd_execute_HMMM(8, y1+y, 256-8, LINES_PER_FRAME, 0, y2+y);
    }
    else if (scroll_x_direction < 0) {
      vdp_cmd_execute_HMMM(0, y1+y, 256-8, LINES_PER_FRAME, 8, y2+y);
    }
    if (!frame_counts) {
      if (0 < scroll_x_direction) {
        vdp_cmd_execute_HMMM(0, y1, 8, LINES, 256-8, y2);
      }
      else if (scroll_x_direction < 0) {
        vdp_cmd_execute_HMMM(256-8, y1, 8, LINES, 0, y2);
      }
    }
  }
}

/**
 * Update the screen (timing critical part).
 * \note This is assumed to be called on VSYNC timing.
 */
void update_on_vsync(void) {
  vdp_set_adjust(-shift_bit, 0);
  select_display_page();
}

/**
 * Update the screen.
 * \note This is assumed to be called on VSYNC timing after update_on_vsync().
 * \attention Cannot be called from interrupt handler since using VDP commands.
 */
void update_after_vsync(void) {
  update_active_page();
}

static void rotate(uint8_t x, uint8_t src_page, uint8_t dst_page) {
  uint16_t y1 = 256 * src_page;
  uint16_t y2 = 256 * dst_page;
  vdp_cmd_execute_HMMM(x, y1, 256-x, LINES, 0, y2);
  vdp_cmd_execute_HMMM(0, y1, x, LINES, 256-x, y2);
}

/**
 * Set X-axis offset for horizontal scroll.
 * \param x  x-axis offset for horizontal scroll (0..255).
 * \note Scroll is done on the next calling to update_after_vsync().
 */
void set_hscroll(uint8_t x) {
  if (scroll_x == x) return;
  uint8_t next_shift_bit = x & 7;
  uint8_t prev_shift_byte = shift_byte;
  uint8_t next_shift_byte = (x >> 3) & 31;
  if (next_shift_byte != prev_shift_byte) {
    int8_t dir = next_shift_byte - prev_shift_byte;
    if (dir < -16) {
      dir += 32;
    }
    else if (15 < dir) {
      dir -= 32;
    }
    // if (dir < 0) {
    //   dir = -1;
    // }
    // else if (0 < dir) {
    //   dir = 1;
    // }
    if (dir != scroll_x_direction) {
      uint8_t p = display_page;
      rotate((uint8_t)(8 * +dir), p, 1 - p);
      rotate((uint8_t)(8 * -dir), 1 - p, p);
      scroll_x_direction = dir;
    }
    frame_counts = FRAMES_PER_8_DOT;
  }
  shift_bit = next_shift_bit;
  shift_byte = next_shift_byte;
  scroll_x = x;
}

// -------------
void vsync_handler(void) {
  update_on_vsync();
  snd_play();
}

#define DECIMAL_BITS (5)
#define MAX_SPEED    ((8 << DECIMAL_BITS) / FRAMES_PER_8_DOT)

void main(void) {
  CLIKSW = 0;
  screen5();
  color(15, 0, 0);
  cls();

  // initialize the screen
  {
    uint8_t c = 0;
    for (uint8_t i = 0; i < LINES / 4; ++i) {
      for (uint8_t j = 0; j < 32; ++j) {
        vdp_cmd_execute_HMMV(8 * j, 4 * i, 8, 4, c);
        c++;
      }
    }

    locate(3, 4); print("ARROW KEY : speed control");
    locate(3, 5); print("SPACE KEY : sound effects");
    locate(4, 10); print("MSX2 Smooth SCROLL DEMO");
    locate(5, 11); print("SCREEN 5 (GRAPHIC 4)");
  }

  uint16_t x = 0;
  int16_t speed = MAX_SPEED;

  snd_init();
  snd_set_repeat(true);
  snd_start();

  snd_set_bgm(0, &bgm);

  // set_vsync_handler(snd_play);
  set_vsync_handler(vsync_handler);
  uint8_t old_pressed = 0;
  for (;;) {
    set_hscroll((x >> DECIMAL_BITS) & 255);
    await_vsync();
    update_after_vsync();
    x += speed;

    uint8_t pressed = joypad_get_state(0);
    uint8_t clicked = (old_pressed ^ pressed) & pressed;
    old_pressed = pressed;

    if (clicked & VK_FIRE_0) {
      // sound effects "coiiiiiiin!"
      snd_set_sfx(1, &bgm);
    }
    if (speed < MAX_SPEED && (pressed & VK_RIGHT)) {
      speed++;
    }
    if (-MAX_SPEED < speed && (pressed & VK_LEFT)) {
      speed--;
    }
  }
}
