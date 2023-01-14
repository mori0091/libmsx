// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2023 Daishi Mori (mori0091)
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

    sleep_ticks(1);             // why needs this??

    // Read 8x8 pixels of the above,
    // and show it in HEX
    vdp_cmd_execute_LMCM(x, y, 8, 8, VDP_CMD_LRTB);
    for (uint8_t i = 0; i < 8; i++) {
      locate(16, 8+i);
      for (uint8_t j = 0; j < 8; j++) {
        uint8_t c;
        vdp_cmd_read(&c);
        text_color(15 - c, c);
        printx(c);
      }
    }
  }
}

// -- LMCM -----------------------------------------------------
// LRTB : copy
// RLTB : horizontal flip
// LRBT : vertical flip
// RLBT : rotate 180 deg
void copy_rect(uint16_t x, uint16_t y,
               uint16_t w, uint16_t h,
               uint16_t dx, uint16_t dy,
               enum vdp_cmd_dir dir) {
  uint16_t sx = x;
  uint16_t sy = y;
  switch (dir) {
    case VDP_CMD_LRTB:
      break;
    case VDP_CMD_RLTB:
      sx += w - 1;
      break;
    case VDP_CMD_LRBT:
      sy += h - 1;
      break;
    case VDP_CMD_RLBT:
      sx += w - 1;
      sy += h - 1;
      break;
  }

  sleep_ticks(1);               // why needs this??

  vmemptr_t p = 128 * dy + dx / 2; // 256 pix width, 4-bpp
  vdp_cmd_execute_LMCM(sx, sy, w, h, dir);
  for (uint16_t i = h; i--; ) {
    vmem_set_write_address(p);
    for (uint16_t j = w / 2; j--; ) {
      // 4-bpp ; 2 pix per byte
      uint8_t c0, c1;
      vdp_cmd_read(&c0);
      vdp_cmd_read(&c1);
      vmem_set((c0 << 4) | (c1 & 15));
    }
    p += 128;
  }
}

void flip(void) {
  color(15, 4, 7);
  cls();

  text_color(15, 1);
  locate(0, 25);
  print("GRAPHIC 4 (SCREEN 5) / LMCM");

  for (int ch = 0; ch < 256; ++ch) {
    fg_color(rand() % 14 + 2);
    putchar_HMMC(ch, (ch & 15) * 8, ch / 16 * 8);
  }

  text_color(15, 1);

  // Copy.
  locate(16, 17); print("Copy            ");
  copy_rect(0, 0, 128, 128, 128, 0, VDP_CMD_LRTB);
  sleep_millis(2500);

  // Horizontal flip.
  locate(16, 17); print("Horizontal flip ");
  copy_rect(0, 0, 128, 128, 128, 0, VDP_CMD_RLTB);
  sleep_millis(2500);

  // Vertical flip.
  locate(16, 17); print("Vertical flip   ");
  copy_rect(0, 0, 128, 128, 128, 0, VDP_CMD_LRBT);
  sleep_millis(2500);

  // Horizontal & Vertical flip.
  locate(16, 17); print("Rotate 180 deg  ");
  copy_rect(0, 0, 128, 128, 128, 0, VDP_CMD_RLBT);
  sleep_millis(2500);
}

// -- LMMC -----------------------------------------------------
// LRTB : transpose
// RLTB : rotate 90 deg cw
// LRBT : rotate 90 deg ccw
// RLBT : diagonal transpose
void copy_rect2(uint16_t x, uint16_t y,
                uint16_t w, uint16_t h,
                uint16_t dx, uint16_t dy,
                enum vdp_cmd_dir dir) {
  uint16_t sx = x;
  uint16_t sy = y;
  int8_t d = 1;
  switch (dir) {
    case VDP_CMD_LRTB:
      break;
    case VDP_CMD_RLTB:
      dx += w - 1;
      d = -1;
      break;
    case VDP_CMD_LRBT:
      dy += h - 1;
      break;
    case VDP_CMD_RLBT:
      dx += w - 1;
      dy += h - 1;
      d = -1;
      break;
  }

  vmemptr_t p = 128 * sy + sx / 2; // 256 pix width, 4-bpp
  for (uint8_t i = 0; i < h; i++) {
    sleep_ticks(1);
    vmem_set_read_address(p);
    vdp_cmd_execute_LMMC(dx, dy, 1, h, dir, VDP_CMD_IMP);
    for (uint8_t j = 0; j < w/2; j++) {
      uint8_t c = vmem_get();
      vdp_cmd_write(c >> 4);
      vdp_cmd_write(c & 15);
    }
    dx += d;
    p += 128;
  }
}

void rotate(void) {
  color(15, 4, 7);
  cls();

  text_color(15, 1);
  locate(0, 25);
  print("GRAPHIC 4 (SCREEN 5) / LMMC");

  for (int ch = 0; ch < 256; ++ch) {
    fg_color(rand() % 14 + 2);
    putchar_HMMC(ch, (ch & 15) * 8, ch / 16 * 8);
  }

  text_color(15, 1);

  // Transpose.
  locate(16, 17); print("Transpose       ");
  copy_rect2(0, 0, 128, 128, 128, 0, VDP_CMD_LRTB);
  sleep_millis(2500);

  // Rotate 90 deg clockwise.
  locate(16, 17); print("Rotate 90deg CW ");
  copy_rect2(0, 0, 128, 128, 128, 0, VDP_CMD_RLTB);
  sleep_millis(2500);

  // Rotate 90 deg counter clockwise
  locate(16, 17); print("Rotate 90deg CCW");
  copy_rect2(0, 0, 128, 128, 128, 0, VDP_CMD_LRBT);
  sleep_millis(2500);

  // Diagonal transpose.
  locate(16, 17); print("Diag. Transpose ");
  copy_rect2(0, 0, 128, 128, 128, 0, VDP_CMD_RLBT);
  sleep_millis(2500);
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
    // Copy, Horizontal/Vertical flip, Rotate 180deg (LMCM)
    flip();
    // Transpose, Rotate 90deg CW/CCW, Diagonal Transpose (LMMC)
    rotate();
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
