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
 * \file scroll.c
 * \brief `MSX` smooth scroll.
 *
 * `MSX` has no hardware scrolling functionality. So (software) smooth scrolling
 * functionality is realized by combination of the following two methods.
 * - Character pattern animation by rewriting the pattern generator table.
 * - 8-dot scroll by rewriting the pattern name table.
 */

#include <msx.h>
#include <screen.h>
#include <stddef.h>
#include <stdint.h>
#include <text.h>

#include <string.h>

// -------------
#define PATTERNS        (0x00000) // pattern generator table
#define IMAGE           (0x01800) // pattern name table
#define COLORS          (0x02000) // color table
#define SIZE_OF_PATTERNS (0x1800) // size of pattern generator table
#define SIZE_OF_IMAGE    (0x0300) // size of pattern name table
#define SIZE_OF_COLORS   (0x1800) // size of color table

#define ROWS (24)
#define COLS (32)

// -------------
// A little faster but unsafe version of vmem_write().
static void vmem_write_unsafe(vmemptr_t loc, const uint8_t * p, size_t len) {
  VDP_SET_VMEM_WRITE_POINTER(loc);
  while (len--) {
    VDP_SET_VMEM_VALUE(*p++);
  }
}

#define vmem_write(loc, p, len)   vmem_write_unsafe(loc, p, len)

// -------------
/**
 * Bitwise shift 8x8 bitmap patterns towards left.
 *
 * \param n     number of patterns. (> 0)
 * \param src   source bitmap patterns. (>= 8 * n bytes)
 * \param dst   destination bitmap patterns. (>= 8 * n bytes)
 * \param shift number of pixels to shift towards left. (<= 8)
 */
static void pat_shift_left(uint8_t n, const uint8_t * src, uint8_t * dst, uint8_t shift) {
  // assert(n > 0);
  // assert(shift <= 8);
  const uint8_t mask = 0xff << (8 - shift);
  for (uint8_t i = 0; i < 8; ++i) {
    uint8_t * q = dst + 8 * (n - 1) + i;
    const uint8_t * p = src + 8 * (n - 1) + i;
    uint8_t b = 0;
    for (uint8_t j = n; j--; ) {
      uint8_t c = (*p << shift) | b;
      b = (*p & mask) >> (8 - shift);
      *q = c;
      p -= 8;
      q -= 8;
    }
  }
}

// -------------
#define __   (0x20)             // SPACE
#define _F   (0x00)             // left buffer of floor
#define FL   (0x01)             // floor (left-end)
#define FM   (0x02)             // floor (mid)
#define FR   (0x03)             // floor (right-end)
#define _B   (0x04)             // left buffer of black box
#define BL   (0x05)             // black box (left-end)
#define BM   (BL)               // black box (mid)
#define BR   (0x06)             // black box (right-end)

const uint8_t default_colors[8] = {
  0xf0, 0xf4, 0xf0, 0xf4, 0xf0, 0xf4, 0xf0, 0xf4,
};

const uint8_t font_colors[][8] = {
  [_F] = { 0xe0, 0x24, 0x30, 0x34, 0xc0, 0xc4, 0xc0, 0x64, }, // left buffer of floor
  [FL] = { 0xe0, 0x24, 0x30, 0x34, 0xc0, 0xc4, 0xc0, 0x64, }, // floor (left end)
  [FM] = { 0xe0, 0x24, 0x30, 0x34, 0xc0, 0xc4, 0xc0, 0x64, }, // floor (mid)
  [FR] = { 0xe0, 0x24, 0x30, 0x34, 0xc0, 0xc4, 0xc0, 0x64, }, // floor (right end)
  [_B] = { 0x10, 0x14, 0x10, 0x14, 0x10, 0x14, 0x10, 0x14, }, // left buffer of black box
  [BL] = { 0x10, 0x14, 0x10, 0x14, 0x10, 0x14, 0x10, 0x14, }, // black box (left end / mid)
  [BR] = { 0x10, 0x14, 0x10, 0x14, 0x10, 0x14, 0x10, 0x14, }, // black box (right end)
};

const uint8_t fonts[][8] = {
  [_F] = {                      // left buffer of floor
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
  },
  [FL] = {                      // floor (left end)
    0b11111111,
    0b11111111,
    0b10100101,
    0b01011010,
    0b01011010,
    0b00100101,
    0b01111111,
    0b00111111,
  },
  [FM] = {                      // floor (mid)
    0b11111111,
    0b11111111,
    0b10100101,
    0b01011010,
    0b01011010,
    0b10100101,
    0b11111111,
    0b11111111,
  },
  [FR] = {                      // floor (right end)
    0b11111111,
    0b11111111,
    0b10100101,
    0b01011010,
    0b01011010,
    0b10100100,
    0b11111110,
    0b11111100,
  },
  [_B] = {                      // left buffer of black box
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
  },
  [BL] = {                      // black box (left end / mid)
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
  },
  [BR] = {                      // black box (right end)
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
  },
};

const uint8_t stage_map[COLS*ROWS] = {
/*        0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31 */
/*  0 */ __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
/*  1 */ __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
/*  2 */ __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
/*  3 */ __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
/*  4 */ __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
/*  5 */ __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
/*  6 */ __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
/*  7 */ __, __, __, __, _F, FL, FM, FM, FM, FM, FM, FM, FR, _F, FL, FR, _F, FL, FM, FM, FR, __, __, __, __, __, __, __, __, __, __, __,

/*  8 */ __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
/*  9 */ __, __, __, _B, BL, BR, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
/* 10 */ __, __, __, _B, BL, BR, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
/* 11 */ __, __, __, _B, BL, BR, __, _B, BL, BR, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
/* 12 */ _F, FL, FM, FM, FM, FR, __, _B, BL, BR, __, __, __, __, __, __, __, __, _F, FL, FM, FM, FM, FM, FM, FR, __, __, __, __, __, __,
/* 13 */ __, __, __, _B, BL, BR, __, _B, BL, BR, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
/* 14 */ __, __, __, _B, BL, BR, __, _B, BL, BR, __, __, __, _B, BL, BR, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
/* 15 */ __, __, __, _B, BL, BR, __, _B, BL, BR, __, __, _B, BL, BL, BL, BL, BR, __, __, __, __, __, __, __, __, __, __, __, __, __, __,

/* 16 */ __, __, __, _B, BL, BR, __, _B, BL, BR, __, __, _B, BL, BL, BL, BL, BR, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
/* 17 */ __, __, __, _B, BL, BR, __, _B, BL, BR, __, _F, FL, FM, FM, FM, FM, FM, FR, __, __, __, __, __, _F, FL, FM, FM, FM, FM, FM, FR,
/* 18 */ __, __, __, _B, BL, BR, __, _B, BL, BR, __, __, _B, BL, BL, BL, BL, BR, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
/* 19 */ __, __, __, _B, BL, BR, __, _B, BL, BR, __, __, _B, BL, BL, BL, BL, BR, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
/* 20 */ __, _B, BR, _B, BL, BR, __, _B, BL, BR, __, __, _B, BL, BL, BL, BL, BR, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
/* 21 */ __, _B, BR, _B, BL, BR, __, _B, BL, BR, __, __, _B, BL, BL, BL, BL, BR, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
/* 22 */ FM, FM, FM, FM, FM, FM, FM, FM, FM, FM, FM, FM, FM, FM, FM, FM, FM, FM, FM, FM, FM, FM, FM, FM, FM, FM, FM, FM, FM, FM, FM, FM,
/* 23 */ _F, FL, FR, __, _F, FL, FR, __, _F, FL, FR, __, _F, FL, FR, __, _F, FL, FR, __, _F, FL, FR, __, _F, FL, FR, __, _F, FL, FR, __,
};

// -------------
static uint8_t font_buffer[8][sizeof(fonts)];

static void setup_scrollable_font(void) {
  // patterns
  for (uint8_t i = 0; i < 8; ++i) {
    pat_shift_left(sizeof(fonts)/sizeof(fonts[0]), &fonts[0][0], &font_buffer[i][0], i);
  }
  // colors
  vmem_write(COLORS+0x0000, (void *)font_colors, sizeof(font_colors));
  vmem_write(COLORS+0x0800, (void *)font_colors, sizeof(font_colors));
  vmem_write(COLORS+0x1000, (void *)font_colors, sizeof(font_colors));
}

static void init_font_colors(void) {
  vmem_set_write_address(COLORS);
  for (uint16_t i = 3*256; i--; ) {
    for (uint8_t j = 0; j < 8; j++) {
      vmem_set(default_colors[j]);
    }
  }
}

// -------------

// frame buffer
static uint8_t fb_data[COLS * ROWS];

// BG plane
static uint8_t bg_data[COLS * ROWS];

/**
 * Copy BG plane scrolled to the frame buffer.
 *
 * \param col  X-axis offset for horizontal 8-dot scroll.
 */
inline void copy_BG_to_FB(uint8_t col) {
  const uint8_t * p = bg_data;
  uint8_t * q = fb_data;
  for (uint8_t i = ROWS; i--; ) {
    memmove(q, p + col, COLS - col);
    memmove(q + COLS - col, p, col);
    p += COLS;
    q += COLS;
  }
}

/**
 * Draw string onto the frame buffer.
 * \param x  column
 * \param y  row
 * \param s  a string
 */
inline void paint_string(uint8_t x, uint8_t y, const char * s) {
  memcpy(fb_data + COLS * y + x, s, strlen(s));
}

/**
 * Draw FG (foreground) plane by overwriting the frame buffer.
 * \note paint() is called back from update().
 */
void paint(void) {
  paint_string(3,  4, "ARROW KEY : speed control");
  paint_string(4, 10,  "MSX1 Smooth SCROLL DEMO");
}

/**
 * Apply one of font patterns to VRAM.
 *
 * \param shift  an index of set of font patterns.
 */
static void update_pattern_generator_table(uint8_t shift) {
  void * p = font_buffer[shift];
  vmem_write(PATTERNS+0x0000, p, sizeof(font_buffer[0]));
  vmem_write(PATTERNS+0x0800, p, sizeof(font_buffer[0]));
  vmem_write(PATTERNS+0x1000, p, sizeof(font_buffer[0]));
}

inline void update_name_table(void) {
  vmem_write(IMAGE, fb_data, COLS * ROWS);
}

static volatile uint8_t scroll_x;

static bool dirty = true;
static bool should_repaint = true;
static uint8_t shift_byte;
static uint8_t shift_bit;

/**
 * Update the screen (for VSYNC frequency = 50Hz).
 * \note This is assumed to be called on VSYNC timing.
 */
void update_50Hz(void) {
  if (!dirty) return;

  if (should_repaint) {
    copy_BG_to_FB(shift_byte);
    paint();
    update_name_table();
  }
  update_pattern_generator_table(shift_bit);

  dirty = false;
}

/**
 * Update the screen (for VSYNC frequency = 60Hz).
 * \note This is assumed to be called on VSYNC timing.
 */
void update_60Hz(void) {
  if (!dirty) return;

  if (should_repaint) {
    copy_BG_to_FB(shift_byte);
    paint();
    update_pattern_generator_table(shift_bit);
    update_name_table();
  }
  else {
    update_pattern_generator_table(shift_bit);
  }

  dirty = false;
}

/**
 * Set X-axis offset for horizontal scroll.
 * \param x  x-axis offset for horizontal scroll (0..255).
 * \note Scroll is done on the next calling to update().
 */
void set_hscroll(uint8_t x) {
  while (dirty)
    ;
  if (scroll_x == x) return;
  shift_bit = x & 7;
  shift_byte = x >> 3;
  should_repaint = (scroll_x >> 3) != shift_byte;
  scroll_x = x;
  dirty = true;
}

void main(void) {
  CLIKSW = 0;
  screen2_PCG();
  color(15, 0, 0);
  cls();

  init_font_colors();
  setup_scrollable_font();
  memcpy(bg_data, stage_map, sizeof(stage_map));

  uint8_t x = 0;
  int8_t speed = 1;

  // Full screen update costs about 40 to 50ms at most, in case of Z80 CPU.
  // This means that scanline passes 2 (if VSYNC=50Hz) or 3 (if VSYNC=60Hz)
  // times during rewriting the screen. Thus, to reduce flickering, need to use
  // dedicated update() function according to the VSYNC frequency.
  // Hmm...
  if (msx_get_vsync_frequency() == 50) {
    set_vsync_handler(update_50Hz);
  }
  else {
    set_vsync_handler(update_60Hz);
  }
  uint16_t t = JIFFY;
  for (;;) {
    set_hscroll(x);
    await_vsync();
    x += speed;

    uint8_t joy = joypad_get_state(0);
    if (speed < 127 && (joy & VK_RIGHT)) {
      speed++;
    }
    if (-127 < speed && (joy & VK_LEFT)) {
      speed--;
    }

    uint16_t tt = JIFFY;
    locate(31, 23);
    printu(tt - t);
    t = tt;
  }
}
