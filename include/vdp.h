// -*- coding: utf-8-unix -*-

#ifndef VDP_H
#define VDP_H

#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#include "workarea.h"

// ---- VDP I/O ports
static volatile __sfr __at 0x98 vdp_port0;
static volatile __sfr __at 0x99 vdp_port1;
static volatile __sfr __at 0x9a vdp_port2;
static volatile __sfr __at 0x9b vdp_port3;

// ---- VRAM access

typedef uint32_t vmemptr_t;

void vmem_set_read_address(vmemptr_t loc);

void vmem_set_write_address(vmemptr_t loc);

inline uint8_t vmem_get(void) {
  return vdp_port0;
}

inline void vmem_set(uint8_t val) {
  vdp_port0 = val;
}

void vmem_read(vmemptr_t src, void* dst, uint16_t len);

void vmem_write(vmemptr_t dst, void* src, uint16_t len);

void vmem_memset(vmemptr_t dst, uint8_t val, uint32_t len);

// ---- VDP status register

uint8_t vdp_get_status(uint8_t reg);

// ---- VDP control registers

void vdp_set_control(uint8_t reg, uint8_t x);

void vdp_write_control(uint8_t reg, void* src, uint8_t len);

// ---- VDP palette registers

typedef uint16_t palette_t;

#define RGB(r, g, b) \
  ((((g) & 7) << 8) | (((r) & 7) << 4) | ((b) & 7))

void vdp_set_palette(uint8_t idx, const palette_t palette);

void vdp_write_palette(const palette_t palettes[16]);

// ---- VDP display setting

enum vdp_screen_mode {
  VDP_SCREEN_MODE_GRAPHIC_1   = 0, // 00000b (00) SCREEN 1
  VDP_SCREEN_MODE_TEXT_1      = 1, // 00001b (01) SCREEN 0: WIDTH 40
  VDP_SCREEN_MODE_MULTI_COLOR = 2, // 00010b (02) SCREEN 3
  VDP_SCREEN_MODE_GRAPHIC_2   = 3, // 00100b (04) SCREEN 2
  VDP_SCREEN_MODE_GRAPHIC_3   = 4, // 01000b (08) SCREEN 4
  VDP_SCREEN_MODE_TEXT_2      = 5, // 01001b (09) SCREEN 0: WIDTH 80
  VDP_SCREEN_MODE_GRAPHIC_4   = 6, // 01100b (0C) SCREEN 5
  VDP_SCREEN_MODE_GRAPHIC_5   = 7, // 10000b (10) SCREEN 6
  VDP_SCREEN_MODE_GRAPHIC_6   = 8, // 10100b (14) SCREEN 7
  VDP_SCREEN_MODE_GRAPHIC_7   = 9, // 11100b (1C) SCREEN 8
};

enum vdp_screen_lines {
  VDP_SCREEN_LINES_192 = 0x00,
  VDP_SCREEN_LINES_212 = 0x80,
};

enum vdp_sprite_size {
  VDP_SPRITE_SIZE_8x8 = 0,
  VDP_SPRITE_SIZE_8x8_MAGNIFIED = 1,
  VDP_SPRITE_SIZE_16x16 = 2,
  VDP_SPRITE_SIZE_16x16_MAGNIFIED = 3,
};

enum vdp_sprite_tag {
  VDP_SPRITE_TAG_IC = (1<<5),   // IC bit (Ignore Collision)
  VDP_SPRITE_TAG_CC = (1<<6),   // CC bit (Compose Color / Cancel Collision)
  VDP_SPRITE_TAG_EC = (1<<7),   // EC bit (Early Clock)
};

void vdp_set_visible(bool visible);

void vdp_set_screen_mode(enum vdp_screen_mode mode);
void vdp_set_screen_lines(enum vdp_screen_lines lines);

void vdp_set_image_table(vmemptr_t table);
void vdp_set_pattern_table(vmemptr_t table);
void vdp_set_color_table(vmemptr_t table);
void vdp_set_sprite_pattern_table(vmemptr_t table);
void vdp_set_sprite_attribute_table(vmemptr_t table);

void vdp_set_sprite_size(enum sprite_size size);
void vdp_set_sprite_visible(bool visible);

void vdp_set_color(uint8_t c);  // set color register R#7

void vdp_set_adjust(int8_t x, int8_t y);

void vdp_set_vscroll(uint8_t y);
void vdp_set_hscroll(uint16_t x);
void vdp_set_hscroll_mask(bool enable);
void vdp_set_hscroll_dual_page(bool enable);

// ---- VDP COMMANDs

inline bool vdp_cmd_is_running(void) {
  return (vdp_get_status(2) & 1);
}

inline void vdp_cmd_await(void) {
  while (vdp_cmd_is_running());
}

void vdp_cmd_set_unrestricted(bool enable);

enum vdp_cmd_logop {
  VDP_CMD_IMP  = 0x00,
  VDP_CMD_AND  = 0x01,
  VDP_CMD_OR   = 0x02,
  VDP_CMD_EOR  = 0x03,
  VDP_CMD_XOR  = VDP_CMD_EOR,
  VDP_CMD_NOT  = 0x04,
  VDP_CMD_TIMP = 0x08,
  VDP_CMD_TAND = 0x09,
  VDP_CMD_TOR  = 0x0a,
  VDP_CMD_TEOR = 0x0b,
  VDP_CMD_TXOR = VDP_CMD_TEOR,
  VDP_CMD_TNOT = 0x0c,
};

void vdp_cmd_execute_LMMV(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                          uint8_t color, enum vdp_cmd_logop logop);

void vdp_cmd_execute_LMMM(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                          uint16_t x2, uint16_t y2, enum vdp_cmd_logop logop);

void vdp_cmd_execute_HMMV(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                          uint8_t color);

void vdp_cmd_execute_HMMM(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                          uint16_t x2, uint16_t y2);

// ---- Sprite

// element of sprite attribute table
struct sprite {
  uint8_t y;
  uint8_t x;
  uint8_t pat;                  // sprite pattern number
  uint8_t tag;                  // EC | CC | IC | 0 | Color Code
};

// element of sprite color table
struct sprite_color {
  uint8_t tags[16];             // EC | CC | IC | 0 | Color Code
};

inline
void sprite_set_x(struct sprite* s, int x) {
  if (x < -32) x = -32;
  if (x > 255) x = 255;
  if (x < 0) {
    s->x = x + 32;
    s->tag |= VDP_SPRITE_TAG_EC;
  } else {
    s->x = x;
    s->tag &= ~VDP_SPRITE_TAG_EC;
  }
}

inline
void sprite_set_y(struct sprite* s, int y) {
  s->y = (uint8_t)(y & 0xFF);
}

inline
void sprite_set_xy(struct sprite* s, int x, int y) {
  sprite_set_y(s, y);
  sprite_set_x(s, x);
}

inline
void sprite_set_pat(struct sprite* s, uint8_t pat) {
  assert(pat < 32);
  s->pat = pat;
}

inline
void sprite_set_color(struct sprite* s, uint8_t color) {
  assert(color < 16);
  s->tag |= color;
}

#endif
