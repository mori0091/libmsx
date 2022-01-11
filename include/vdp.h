// -*- coding: utf-8-unix -*-
/**
 * \file vdp.h
 *
 * Copyright (c) 2021 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

#ifndef VDP_H
#define VDP_H

#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#include "io.h"
#include "vmem.h"

#include "bios.h"
#include "workarea.h"

inline void VDP_SET_CONTROL_REGISTER(uint8_t reg, uint8_t val) {
  vdp_port1 = val;
  vdp_port1 = reg | 0x80;
}

inline void VDP_SET_CONTROL_REGISTER_POINTER(uint8_t reg) {
  VDP_SET_CONTROL_REGISTER(17, reg | 0x80);
}

inline void VDP_SET_CONTROL_REGISTER_POINTER_AUTO_INCREMENT(uint8_t reg) {
  VDP_SET_CONTROL_REGISTER(17, reg);
}

inline void VDP_SET_CONTROL_REGISTER_VALUE(uint8_t val) {
  vdp_port3 = val;
}

inline void VDP_SET_STATUS_REGISTER_POINTER(uint8_t reg) {
  VDP_SET_CONTROL_REGISTER(15, reg);
}

inline uint8_t VDP_GET_STATUS_REGISTER_VALUE(void) {
  return vdp_port1;
}

inline void VDP_SET_VMEM_WRITE_POINTER(vmemptr_t loc) {
  if (0 < msx_get_version()) {
    VDP_SET_CONTROL_REGISTER(14, (uint8_t)(((loc) >> 14) & 7));
  }
  vdp_port1 = (uint8_t)((loc) & 255);
  vdp_port1 = (uint8_t)(((loc) >> 8) & 0x3F | 0x40);
}

inline void VDP_SET_VMEM_READ_POINTER(vmemptr_t loc) {
  if (0 < msx_get_version()) {
    VDP_SET_CONTROL_REGISTER(14, (uint8_t)(((loc) >> 14) & 7));
  }
  vdp_port1 = (uint8_t)((loc) & 255);
  vdp_port1 = (uint8_t)(((loc) >> 8) & 0x3F);
}

inline void VDP_SET_VMEM_VALUE(uint8_t val) {
  vdp_port0 = val;
}

inline uint8_t VDP_GET_VMEM_VALUE(void) {
  return vdp_port0;
}

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

void vdp_set_visible(bool visible);

uint8_t vdp_get_sprite_mode(void);
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
  while (vdp_cmd_is_running())
    ;
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

/**
 * Enumeration of operation codes for VDP commands.
 *
 * \sa vdp_cmd_execute()
 */
enum vdp_cmd_op {
  /** Operation code "STOP" : (not implemented) */
  VDP_CMD_STOP = 0x00,
  /** Operation code "POINT" : (not implemented) */
  VDP_CMD_POINT = 0x40,
  /** Operation code "PSET" : (not implemented) */
  VDP_CMD_PSET = 0x50,
  /** Operation code "SRCH" : (not implemented) */
  VDP_CMD_SRCH = 0x60,
  /** Operation code "LINE" : (not implemented) */
  VDP_CMD_LINE = 0x70,
  /** Operation code "LMMV" : Fill rectangular area w/ logical operation. */
  VDP_CMD_LMMV = 0x80,
  /** Operation code "LMMM" : Copy rectangular area from VRAM to VRAM w/ logical operation. */
  VDP_CMD_LMMM = 0x90,
  /** Operation code "LMCM" : (not implemented) */
  VDP_CMD_LMCM = 0xa0,
  /** Operation code "LMMC" : (not implemented) */
  VDP_CMD_LMMC = 0xb0,
  /** Operation code "HMMV" : Fill rectangular area. */
  VDP_CMD_HMMV = 0xc0,
  /** Operation code "HMMM" : Copy rectangular area from VRAM to VRAM */
  VDP_CMD_HMMM = 0xd0,
  /** Operation code "YMMM" : (not implemented) */
  VDP_CMD_YMMM = 0xe0,
  /** Operation code "HMMC" : (not implemented) */
  VDP_CMD_HMMC = 0xf0,
};

/**
 * Parameters for VDP commands.
 *
 * \sa vdp_cmd_execute()
 */
struct vdp_cmd {
  uint8_t r32;            // SX (lo)
  uint8_t r33;            // SX (hi)
  uint8_t r34;            // SY (lo)
  uint8_t r35;            // SY (hi)
  uint8_t r36;            // DX (lo)
  uint8_t r37;            // DX (hi)
  uint8_t r38;            // DY (lo)
  uint8_t r39;            // DY (hi)
  uint8_t r40;            // NX (lo)
  uint8_t r41;            // NX (hi)
  uint8_t r42;            // NY (lo)
  uint8_t r43;            // NY (hi)
  uint8_t r44;            // CLR (2bpp * 4 pix, 4bpp * 2 pix, or 8bpp * 1 pix)
  uint8_t r45;            // ARG (0000b | DIY | DIX | 00b)
  uint8_t r46;            // CMR (op | logop)
};

/**
 * Set SX value (x-coordinate of the source point) for VDP commands.
 *
 * \param c   pointer to `struct vdp_cmd`.
 * \param sx  SX value.
 *
 * \sa vdp_cmd_execute()
 */
inline void vdp_cmd_set_SX(struct vdp_cmd * c, uint16_t sx) {
  c->r32 = (sx & 0xff);
  c->r33 = (sx >> 8) & 0x01;
}

/**
 * Set SY value (y-coordinate of the source point) for VDP commands.
 *
 * \param c   pointer to `struct vdp_cmd`.
 * \param sy  SY value.
 *
 * \sa vdp_cmd_execute()
 */
inline void vdp_cmd_set_SY(struct vdp_cmd * c, uint16_t sy) {
  c->r34 = (sy & 0xff);
  c->r35 = (sy >> 8) & 0x03;
}

/**
 * Set DX value (x-coordinate of the destination point) for VDP commands.
 *
 * \param c   pointer to `struct vdp_cmd`.
 * \param dx  DX value.
 *
 * \sa vdp_cmd_execute()
 */
inline void vdp_cmd_set_DX(struct vdp_cmd * c, uint16_t dx) {
  c->r36 = (dx & 0xff);
  c->r37 = (dx >> 8) & 0x01;
}

/**
 * Set DY value (y-coordinate of the destination point) for VDP commands.
 *
 * \param c   pointer to `struct vdp_cmd`.
 * \param dy  DY value.
 *
 * \sa vdp_cmd_execute()
 */
inline void vdp_cmd_set_DY(struct vdp_cmd * c, uint16_t dy) {
  c->r38 = (dy & 0xff);
  c->r39 = (dy >> 8) & 0x03;
}

/**
 * Set NX value (width) for VDP commands.
 *
 * \param c   pointer to `struct vdp_cmd`.
 * \param nx  NX value.
 *
 * \sa vdp_cmd_execute()
 */
inline void vdp_cmd_set_NX(struct vdp_cmd * c, uint16_t nx) {
  c->r40 = (nx & 0xff);
  c->r41 = (nx >> 8) & 0x01;
}

/**
 * Set NY value (height) for VDP commands.
 *
 * \param c   pointer to `struct vdp_cmd`.
 * \param ny  NY value.
 *
 * \sa vdp_cmd_execute()
 */
inline void vdp_cmd_set_NY(struct vdp_cmd * c, uint16_t ny) {
  c->r42 = (ny & 0xff);
  c->r43 = (ny >> 8) & 0x03;
}

/**
 * Set CLR value (color) for VDP commands.
 *
 * \param c   pointer to `struct vdp_cmd`.
 * \param clr CLR value.
 *
 * \sa vdp_cmd_execute()
 */
inline void vdp_cmd_set_CLR(struct vdp_cmd * c, uint8_t clr) {
  c->r44 = clr;
}

/**
 * Set ARG value (DIX, DIY, etc.) for VDP commands.
 *
 * \param c   pointer to `struct vdp_cmd`.
 * \param arg ARG value.
 *
 * \sa vdp_cmd_execute()
 */
inline void vdp_cmd_set_ARG(struct vdp_cmd * c, uint8_t arg) {
  c->r45 = arg;
}

/**
 * Set a logical operation code for VDP commands.
 *
 * \param c      pointer to `struct vdp_cmd`.
 * \param logop  a logical operation code.
 *
 * \sa vdp_cmd_execute()
 */
inline void vdp_cmd_set_logop(struct vdp_cmd * c, enum vdp_cmd_logop logop) {
  c->r46 = logop;
}

/**
 * Executes a VDP command.
 *
 * Requests the VDP to execute a VDP command. When a previous VDP command is
 * still running, this function waits for the command to finish before
 * requesting. Therefore you need not to call vdp_cmd_await() before calling
 * vdp_cmd_execute().
 *
 * Before calling this function, all parameters for the VDP command must be set
 * to the `struct vdp_cmd` object pointed by `c`. To set parameters, you may use
 * vdp_cmd_set_*() inline functions.
 *
 * \param c       pointer to `struct vdp_cmd`.
 * \param opcode  an operation code
 */
void vdp_cmd_execute(const struct vdp_cmd * c, enum vdp_cmd_op opcode);

void vdp_cmd_execute_LMMV(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                          uint8_t color, enum vdp_cmd_logop logop);

void vdp_cmd_execute_LMMM(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                          uint16_t x2, uint16_t y2, enum vdp_cmd_logop logop);

void vdp_cmd_execute_HMMV(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                          uint8_t color);

void vdp_cmd_execute_HMMM(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                          uint16_t x2, uint16_t y2);


#endif
