// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2022 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file graphics.c
 * \brief `MSX2` VDP command usecase.
 */

#include <msx.h>
#include <screen.h>
#include <stdint.h>
#include <text.h>

#include <stdlib.h>

// -- HMMC / LMMC ----------------------------------------------
void putchar_HMMC(uint8_t c, uint16_t x, uint16_t y) {
  const uint8_t * p = &CGTBL->data[c][0];
  vdp_cmd_execute_HMMC(x, y, 8, 8, VDP_CMD_LRTB);
  for (uint8_t n = 8; n--; ) {
    uint8_t bits = *p++;
    for (uint8_t i = 4; i--; ) {
      uint8_t color
        = (((bits & 0x80) ? FORCLR : BAKCLR) << 4)
        | ((bits & 0x40) ? FORCLR : BAKCLR);
      vdp_cmd_write(color);
      bits <<= 2;
    }
  }
}

void putchar_LMMC(uint8_t c, uint16_t x, uint16_t y) {
  const uint8_t * p = &CGTBL->data[c][0];
  vdp_cmd_execute_LMMC(x, y, 8, 8, VDP_CMD_LRTB, VDP_CMD_TIMP);
  for (uint8_t n = 8; n--; ) {
    uint8_t bits = *p++;
    for (uint8_t i = 8; i--; ) {
      vdp_cmd_write((bits & 0x80) ? FORCLR : 0);
      bits <<= 1;
    }
  }
}

void print_HMMC(const char * s, uint16_t x, uint16_t y) {
  while (*s) {
    if (255 < x) {
      x &= 255;
      y += 8;
    }
    if (192 < y) {
      y -= 192;
    }
    putchar_HMMC(*s++, x, y);
    x += 8;
  }
}

void print_LMMC(const char * s, uint16_t x, uint16_t y) {
  while (*s) {
    if (255 < x) {
      x &= 255;
      y += 8;
    }
    if (192 < y) {
      y -= 192;
    }
    putchar_LMMC(*s++, x, y);
    x += 8;
  }
}

void random_print(uint8_t LorH, const char * s) {
  color(15, 4, 7);
  cls();

  text_color(15, 1);
  locate(0, 25);
  if (LorH) {
    print("GRAPHIC 4 (SCREEN 5) / HMMC");
  }
  else {
    print("GRAPHIC 4 (SCREEN 5) / LMMC");
  }

  const uint16_t n = 10 * msx_get_vsync_frequency(); // 10 sec
  const uint16_t t0 = JIFFY;
  for (; JIFFY - t0 < n; ) {
    uint16_t x = rand() % 256;
    uint16_t y = rand() % 200;
    uint8_t fg = rand() % 16;
    uint8_t bg = rand() % 16;
    text_color(fg, bg);
    if (LorH) {
      print_HMMC(s, x, y);
    }
    else {
      print_LMMC(s, x, y);
    }
  }
}

// -- LMCM -----------------------------------------------------
static uint8_t buf[8*8];

void read_image_LMCM(void) {
  color(15, 4, 7);
  cls();

  text_color(15, 1);
  locate(0, 25);
  print("GRAPHIC 4 (SCREEN 5) / LMCM");

  const uint16_t n = 10 * msx_get_vsync_frequency(); // 10 sec
  const uint16_t t0 = JIFFY;
  for (; JIFFY - t0 < n; ) {
    // print a character
    const uint8_t ch = rand() % 256;
    const uint8_t fg = rand() % 16;
    const uint8_t bg = rand() % 16;
    const uint16_t x = 8 * 7;
    const uint16_t y = 8 * 12;
    text_color(fg, bg);
    putchar_HMMC(ch, x, y);
    sleep_ticks(2);             // why needs this??

    // Read 8x8 pixels of the above.
    uint8_t *p = &buf[0];
    uint8_t m = sizeof(buf);
    vdp_cmd_execute_LMCM(x, y, 8, 8, VDP_CMD_LRTB);
    while (m--) {
      vdp_cmd_read(p++);
    }

    // Show it in HEX
    for (uint8_t i = 0; i < 8; i++) {
      locate(16, 8+i);
      for (uint8_t j = 0; j < 8; j++) {
        uint8_t c = buf[i*8+j];
        text_color(15 - c, c);
        printx(c);
      }
    }
  }
}

// -- PSET -----------------------------------------------------
void random_dot(void) {
  color(15, 4, 7);
  cls();

  text_color(15, 1);
  locate(0, 25);
  print("GRAPHIC 4 (SCREEN 5) / PSET");

  const uint16_t n = 10 * msx_get_vsync_frequency(); // 10 sec
  const uint16_t t0 = JIFFY;
  for (; JIFFY - t0 < n; ) {
    uint16_t x = rand() % 256;
    uint16_t y = rand() % 200;
    uint8_t color = rand() % 16;
    vdp_cmd_execute_PSET(x, y, color, VDP_CMD_IMP);
  }
}

// -- LINE -----------------------------------------------------
void random_line(void) {
  color(15, 4, 7);
  cls();

  text_color(15, 1);
  locate(0, 25);
  print("GRAPHIC 4 (SCREEN 5) / LINE");

  const uint16_t n = 10 * msx_get_vsync_frequency(); // 10 sec
  const uint16_t t0 = JIFFY;
  for (; JIFFY - t0 < n; ) {
    uint16_t x1 = rand() % 256;
    uint16_t x2 = rand() % 256;
    uint16_t y1 = rand() % 200;
    uint16_t y2 = rand() % 200;
    uint8_t color = rand() % 16;
    vdp_cmd_execute_LINE(x1, y1, x2, y2, color, VDP_CMD_IMP);
  }
}

// -- HMMV / LMMV ----------------------------------------------
inline int diff(int a, int b) {
  return (a < b ? b - a : a - b);
}

inline int min(int a, int b) {
  return (a < b ? a : b);
}

void random_box(uint8_t LorH) {
  color(15, 4, 7);
  cls();

  text_color(15, 1);
  locate(0, 25);
  if (LorH) {
    print("GRAPHIC 4 (SCREEN 5) / HMMV");
  }
  else {
    print("GRAPHIC 4 (SCREEN 5) / LMMV");
  }

  const uint16_t n = 10 * msx_get_vsync_frequency(); // 10 sec
  const uint16_t t0 = JIFFY;
  for (; JIFFY - t0 < n; ) {
    uint16_t x1 = rand() % 256;
    uint16_t x2 = rand() % 256;
    uint16_t y1 = rand() % 200;
    uint16_t y2 = rand() % 200;
    uint8_t color = rand() % 16;
    // if (x1 == x2 || y1 == y2) {
    //   continue;
    // }
    if (LorH) {
      // 4-bpp ; 2 pix per byte
      x1 &= ~1;
      x2 &= ~1;
      uint16_t w = diff(x1, x2) + 2;
      uint16_t h = diff(y1, y2) + 1;
      vdp_cmd_execute_HMMV(min(x1, x2), min(y1, y2), w, h, color * 0x11);
    }
    else {
      uint16_t w = diff(x1, x2) + 1;
      uint16_t h = diff(y1, y2) + 1;
      vdp_cmd_execute_LMMV(min(x1, x2), min(y1, y2), w, h, color, VDP_CMD_IMP);
    }
  }
}

// -- HMMM / LMMM ----------------------------------------------
void rect_move(uint8_t LorH) {
  color(15, 4, 7);
  cls();

  text_color(15, 1);
  locate(0, 25);
  if (LorH) {
    print("GRAPHIC 4 (SCREEN 5) / HMMM");
  }
  else {
    print("GRAPHIC 4 (SCREEN 5) / LMMM");
  }

  const uint16_t w = 32;
  const uint16_t h = 32;
  int16_t x = (256 - w) / 2;
  int16_t y = (212 - h) / 2;
  int vx = -1;
  int vy = -1;
  vdp_cmd_execute_LMMV(x, y, w, h, 9, VDP_CMD_IMP);
  vdp_cmd_execute_LMMV(x+2, y+2, w-4, h-4, 4, VDP_CMD_IMP);
  vdp_cmd_execute_LMMV(x+4, y+4, w-8, h-8, 2, VDP_CMD_IMP);

  const uint16_t n = 10 * msx_get_vsync_frequency(); // 10 sec
  const uint16_t t0 = JIFFY;
  for (; JIFFY - t0 < n; ) {
    uint16_t x0 = x;
    uint16_t y0 = y;
    x += vx;
    y += vy;
    if (x < 0) {
      x = 0;
      vx = -vx;
    }
    if (256 - w <= x) {
      x = 256 - w;
      vx = -vx;
    }
    if (y < 0) {
      y = 0;
      vy = -vy;
    }
    if (200 - h <= y) {
      y = 200 - h;
      vy = -vy;
    }
    // await_vsync();
    if (LorH) {
      vdp_cmd_execute_HMMM(x0, y0, w, h, x, y);
    }
    else {
      vdp_cmd_execute_LMMM(x0, y0, w, h, x, y, VDP_CMD_IMP);
    }
  }
}

// -- YMMM -----------------------------------------------------
void rect_move_v(void) {
  color(15, 4, 7);
  cls();

  text_color(15, 1);
  locate(0, 25);
  print("GRAPHIC 4 (SCREEN 5) / YMMM");

  const uint16_t h = 32;
  int16_t yl = (212 - h) / 2;
  int16_t yr = yl;
  int vyl = -1;
  int vyr = 1;
  vdp_cmd_execute_HMMV(0, yl + 1, 256, h - 2, 0x22);

  const uint16_t n = 10 * msx_get_vsync_frequency(); // 10 sec
  const uint16_t t0 = JIFFY;
  for (; JIFFY - t0 < n; ) {
    uint16_t yl0 = yl;
    uint16_t yr0 = yr;
    yl += vyl;
    yr += vyr;
    if (yl < 0) {
      yl = 0;
      vyl = -vyl;
    }
    if (200 - h <= yl) {
      yl = 200 - h;
      vyl = -vyl;
    }
    if (yr < 0) {
      yr = 0;
      vyr = -vyr;
    }
    if (200 - h <= yr) {
      yr = 200 - h;
      vyr = -vyr;
    }
    vdp_cmd_execute_YMMM(127, yl0, h, yl, 4); // left half box
    vdp_cmd_execute_YMMM(128, yr0, h, yr, 0); // right half box
  }
}

// --------------------------------------------------------------
void main(void) {
  screen5();
  for (;;) {
    // Random print (LMMC)
    random_print(0, "HELLO");
    // Random print (HMMC)
    random_print(1, "HELLO");
    // Read rectanguler area (LMCM)
    read_image_LMCM();
    // Random dot (PSET)
    random_dot();
    // Random lines (LINE)
    random_line();
    // Random Box (LMMV)
    random_box(0);
    // Random Box (HMMV)
    random_box(1);
    // Move Rectangle (LMMM)
    rect_move(0);
    // Move Rectangle (HMMM)
    rect_move(1);
    // Move Rectangle vertically (YMMM)
    rect_move_v();
  }
}
