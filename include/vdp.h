// -*- coding: utf-8-unix -*-
/**
 * \file vdp.h
 * \brief Data types and functions for VDP (Video Display Proccessor) access.
 *
 * Copyright (c) 2021 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
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

/**
 * Read from a VDP status register.
 *
 * \param reg  VDP status register number.
 * \return     Value of the VDP status register.
 */
uint8_t vdp_get_status(uint8_t reg);

// ---- VDP control registers

/**
 * Write to a VDP control register.
 *
 * \param reg  VDP control register number.
 * \param x    Value to be written.
 */
void vdp_set_control(uint8_t reg, uint8_t x);

/**
 * Write to a series of VDP control registers.
 *
 * \param reg  First number of the VDP control register.
 * \param src  Pointer to the beginning of the value to be written.
 * \param len  Number of VDP control registers and values.
 */
void vdp_write_control(uint8_t reg, void* src, uint8_t len);

// ---- VDP palette registers

/**
 * Type for RGB color palette value.
 *
 * \sa RGB()
 */
typedef uint16_t palette_t;

/**
 * Constructs RGB color palette value.
 *
 * \param r  Red component (0..7)
 * \param g  Green component (0..7)
 * \param b  Blue component (0..7)
 * \return   RGB color palette value.
 */
#define RGB(r, g, b)                                              \
  ((palette_t)((((g) & 7) << 8) | (((r) & 7) << 4) | ((b) & 7)))

/**
 * Write to a VDP palette register.
 *
 * \param idx      color palette number. (0..15)
 * \param palette  color palette value.
 *
 * \sa RGB()
 */
void vdp_set_palette(uint8_t idx, const palette_t palette);

/**
 * Write to a series of VDP palette registers.
 *
 * \param palettes  16 color palette values.
 *
 * \sa RGB()
 */
void vdp_write_palette(const palette_t palettes[16]);

// ---- VDP display setting

/**
 * Enumeration of VDP screen modes.
 * \sa vdp_set_screen_mode()
 */
enum vdp_screen_mode {
  /** GRAPHIC 1 (SCREEN 1) */
  VDP_SCREEN_MODE_GRAPHIC_1   = 0, // 00000b (00) SCREEN 1
  /** TEXT 1 (SCREEN 0, WIDTH 40)*/
  VDP_SCREEN_MODE_TEXT_1      = 1, // 00001b (01) SCREEN 0: WIDTH 40
  /** MULTI COLOR (SCREEN 3) */
  VDP_SCREEN_MODE_MULTI_COLOR = 2, // 00010b (02) SCREEN 3
  /** GRAPHIC 2 (SCREEN 2) */
  VDP_SCREEN_MODE_GRAPHIC_2   = 3, // 00100b (04) SCREEN 2
  /** GRAPHIC 3 (SCREEN 4) */
  VDP_SCREEN_MODE_GRAPHIC_3   = 4, // 01000b (08) SCREEN 4
  /** TEXT 2 (SCREEN 0, WIDTH 80)*/
  VDP_SCREEN_MODE_TEXT_2      = 5, // 01001b (09) SCREEN 0: WIDTH 80
  /** GRAPHIC 4 (SCREEN 5) */
  VDP_SCREEN_MODE_GRAPHIC_4   = 6, // 01100b (0C) SCREEN 5
  /** GRAPHIC 5 (SCREEN 6) */
  VDP_SCREEN_MODE_GRAPHIC_5   = 7, // 10000b (10) SCREEN 6
  /** GRAPHIC 6 (SCREEN 7) */
  VDP_SCREEN_MODE_GRAPHIC_6   = 8, // 10100b (14) SCREEN 7
  /** GRAPHIC 7 (SCREEN 8) */
  VDP_SCREEN_MODE_GRAPHIC_7   = 9, // 11100b (1C) SCREEN 8
};

/**
 * Enumeration of VDP screen lines.
 * \sa vdp_set_screen_lines()
 */
enum vdp_screen_lines {
  /** 192 lines */
  VDP_SCREEN_LINES_192 = 0x00,
  /** 212 lines */
  VDP_SCREEN_LINES_212 = 0x80,
};

/**
 * Enumeration of VDP sprite size.
 * \sa vdp_set_sprite_size()
 */
enum vdp_sprite_size {
  /** 8x8 pixels */
  VDP_SPRITE_SIZE_8x8 = 0,
  /** 8x8 pixels, 2x magnification */
  VDP_SPRITE_SIZE_8x8_MAGNIFIED = 1,
  /** 16x16 pixels */
  VDP_SPRITE_SIZE_16x16 = 2,
  /** 16x16 pixels, 2x magnification */
  VDP_SPRITE_SIZE_16x16_MAGNIFIED = 3,
};

/**
 * Show / hide screen.
 *
 * \param visible  show screen if `true`, hide otherwise.
 */
void vdp_set_visible(bool visible);

/**
 * Get current sprite mode.
 *
 * The sprite mode will be determined based on the current screen mode.
 *
 * | `enum vdp_screen_mode`        | VDP Screen mode   | BASIC Screen mode  | Sprite mode | compatibility |
 * | ----------------------------- | ----------------- | ------------------ | ----------- | ------------- |
 * | `VDP_SCREEN_MODE_TEXT_1`      | TEXT 1            | SCREEN 0, WIDTH 40 | (no sprite) | MSX           |
 * | `VDP_SCREEN_MODE_GRAPHIC_1`   | GRAPHIC 1         | SCREEN 1           | **mode 1**  | MSX           |
 * | `VDP_SCREEN_MODE_GRAPHIC_2`   | GRAPHIC 2         | SCREEN 2           | **mode 1**  | MSX           |
 * | `VDP_SCREEN_MODE_MULTI_COLOR` | MULTI COLOR       | SCREEN 3           | **mode 1**  | MSX           |
 * | `VDP_SCREEN_MODE_TEXT_2`      | TEXT 2            | SCREEN 0, WIDTH 80 | (no sprite) | MSX2          |
 * | `VDP_SCREEN_MODE_GRAPHIC_3`   | GRAPHIC 3         | SCREEN 4           | **mode 2**  | MSX2          |
 * | `VDP_SCREEN_MODE_GRAPHIC_4`   | GRAPHIC 4         | SCREEN 5           | **mode 2**  | MSX2          |
 * | `VDP_SCREEN_MODE_GRAPHIC_5`   | GRAPHIC 5         | SCREEN 6           | **mode 2**  | MSX2          |
 * | `VDP_SCREEN_MODE_GRAPHIC_6`   | GRAPHIC 6         | SCREEN 7           | **mode 2**  | MSX2          |
 * | `VDP_SCREEN_MODE_GRAPHIC_7`   | GRAPHIC 7         | SCREEN 8           | **mode 2**  | MSX2          |
 * | `VDP_SCREEN_MODE_GRAPHIC_7`   | GRAPHIC 7 YJK/RGB | SCREEN 10 or 11    | **mode 2**  | MSX2+         |
 * | `VDP_SCREEN_MODE_GRAPHIC_7`   | GRAPHIC 7 YJK     | SCREEN 12          | **mode 2**  | MSX2+         |
 *
 * \return  The sprite mode based on the current screen mode:
 *          - 0 if the screen mode was TEXT 1 or TEXT 2,
 *          - 1 if the sprite mode was mode 1,
 *          - 2 if the sprite mode was mode 2.
 *
 * \note
 * If you call this function before calling vdp_set_screen_mode(), the return
 * value may be wrong.
 *
 * \sa vdp_set_screen_mode()
 */
uint8_t vdp_get_sprite_mode(void);

/**
 * Set VDP screen mode.
 *
 * | `enum vdp_screen_mode`        | VDP Screen mode   | BASIC Screen mode  | Sprite mode | compatibility |
 * | ----------------------------- | ----------------- | ------------------ | ----------- | ------------- |
 * | `VDP_SCREEN_MODE_TEXT_1`      | TEXT 1            | SCREEN 0, WIDTH 40 | (no sprite) | MSX           |
 * | `VDP_SCREEN_MODE_GRAPHIC_1`   | GRAPHIC 1         | SCREEN 1           | mode 1      | MSX           |
 * | `VDP_SCREEN_MODE_GRAPHIC_2`   | GRAPHIC 2         | SCREEN 2           | mode 1      | MSX           |
 * | `VDP_SCREEN_MODE_MULTI_COLOR` | MULTI COLOR       | SCREEN 3           | mode 1      | MSX           |
 * | `VDP_SCREEN_MODE_TEXT_2`      | TEXT 2            | SCREEN 0, WIDTH 80 | (no sprite) | MSX2          |
 * | `VDP_SCREEN_MODE_GRAPHIC_3`   | GRAPHIC 3         | SCREEN 4           | mode 2      | MSX2          |
 * | `VDP_SCREEN_MODE_GRAPHIC_4`   | GRAPHIC 4         | SCREEN 5           | mode 2      | MSX2          |
 * | `VDP_SCREEN_MODE_GRAPHIC_5`   | GRAPHIC 5         | SCREEN 6           | mode 2      | MSX2          |
 * | `VDP_SCREEN_MODE_GRAPHIC_6`   | GRAPHIC 6         | SCREEN 7           | mode 2      | MSX2          |
 * | `VDP_SCREEN_MODE_GRAPHIC_7`   | GRAPHIC 7         | SCREEN 8           | mode 2      | MSX2          |
 * | `VDP_SCREEN_MODE_GRAPHIC_7`   | GRAPHIC 7 YJK/RGB | SCREEN 10 or 11    | mode 2      | MSX2+         |
 * | `VDP_SCREEN_MODE_GRAPHIC_7`   | GRAPHIC 7 YJK     | SCREEN 12          | mode 2      | MSX2+         |
 *
 * \param mode  VDP screen mode.
 *
 * \note
 * This function just set/reset the M1..M5 bits of the VDP mode registers,
 * any other settings are not changed by this function.
 *
 * \note
 * Therefore you should do the following after this function if needed:
 * - Address of **pattern name table** shall be set by calling
 *   vdp_set_image_table().
 * - Address of **pattern generator table** shall be set by calling
 *   vdp_set_pattern_table().
 * - Address of **color table** shall be set by calling vdp_set_color_table().
 * - Address of **sprite pattern generator table** shall be set by calling
 *   vdp_set_sprite_pattern_table().
 * - Address of **sprite attribute table** shall be set by calling
 *   vdp_set_sprite_attribute_table().
 * - **Sprite size** shall be set by calling vdp_set_sprite_size().
 * - In case of MSX2 or later, **number of visible lines** shall be set by calling
 *   vdp_set_screen_lines().
 * - In case of MSX2+ or later and If you want to use YJK mode of V9958 (i.e.,
 *   SCREEN 10, 11, or 12), **YJK/YAE bits** shall be set manually by calling
 *   vdp_set_control(). (There is no dedicated functions for setting YJK/YAE bits.)
 * - etc.
 *
 * \sa vdp_set_image_table()
 * \sa vdp_set_pattern_table()
 * \sa vdp_set_color_table()
 * \sa vdp_set_screen_lines()
 * \sa vdp_set_sprite_pattern_table()
 * \sa vdp_set_sprite_attribute_table()
 * \sa vdp_set_sprite_size()
 * \sa vdp_get_sprite_mode()
 */
void vdp_set_screen_mode(enum vdp_screen_mode mode);

/**
 * Set number of visible lines.
 *
 * \param lines  `VDP_SCREEN_LINES_192` for 192 lines, or \n
 *               `VDP_SCREEN_LINES_212` for 212 lines.
 */
void vdp_set_screen_lines(enum vdp_screen_lines lines);

/**
 * Set VRAM address of the pattern name table.
 *
 * \param table  VRAM address of the pattern name table.
 *
 * \note
 * **Pattern name table** is almost the same as what is called
 * a **frame buffer** in modern computer systems.
 *
 * \note
 * Especially in the bitmap graphics mode (GRAPHIC 4 to 7), the pattern name
 * table serves as a frame buffer for 2bpp, 4bpp, or 8bpp graphics images.
 *
 * \note
 * In any other screen mode, it serves as a frame buffer for tile pattern
 * matrix.
 */
void vdp_set_image_table(vmemptr_t table);

/**
 * Set VRAM address of the pattern generator table.
 *
 * \param table  VRAM address of the pattern generator table.
 *
 * \note
 * In the bitmap graphics mode (GRAPHIC 4 to 7), you may not call this function.
 */
void vdp_set_pattern_table(vmemptr_t table);

/**
 * Set VRAM address of the color table.
 *
 * \param table  VRAM address of the color table.
 *
 * \note
 * In the bitmap graphics mode (GRAPHIC 4 to 7), you may not call this function.
 */
void vdp_set_color_table(vmemptr_t table);

/**
 * Set VRAM address of the sprite pattern generator table.
 *
 * \param table  VRAM address of the sprite pattern generator table.
 */
void vdp_set_sprite_pattern_table(vmemptr_t table);

/**
 * Set VRAM address of the sprite attribute table.
 *
 * \param table  VRAM address of the sprite attribute table.
 *
 * \note
 * In sprite mode 2, there is one more table called "sprite color table".
 * \note
 * The sprite color table is (implicitlly) placed at `SAT - 0x0200` ; where
 * `SAT` is the base address of sprite attribute table.
 */
void vdp_set_sprite_attribute_table(vmemptr_t table);

/**
 * Set sprite size.
 *
 * \param size  sprite size.
 */
void vdp_set_sprite_size(enum vdp_sprite_size size);

/**
 * Show / hide sprites.
 *
 * \param visible  show sprites if `true`, hide otherwise.
 */
void vdp_set_sprite_visible(bool visible);

/**
 * Set VDP color register.
 *
 * \param c  value to be set to VDP color register (R#7)
 */
void vdp_set_color(uint8_t c);  // set color register R#7

/**
 * Set VDP display adjust register.
 *
 * - If x < 0, shift the display area to left.
 * - If x > 0, shift the display area to right.
 * - If y < 0, shift the display area to upper.
 * - If y > 0, shift the display area to lower.
 *
 * \param x  x offset (-7..+8)
 * \param y  y offset (-7..+8)
 */
void vdp_set_adjust(int8_t x, int8_t y);

/**
 * Set VDP vertical display offset register.
 *
 * \param y  vertical offset. (0..255)
 */
void vdp_set_vscroll(uint8_t y);

/**
 * Set VDP horizontal display offset register.
 *
 * \param x  horizontal offset (0..511)
 */
void vdp_set_hscroll(uint16_t x);

/**
 * Enables/Disables the screen mask for the leftmost 8 pixels.
 *
 * \param enable  Hide the leftmost 8 pixels if `true`, show otherwise.
 */
void vdp_set_hscroll_mask(bool enable);

/**
 * Enable/disable horizontal scrolling for two pages.
 *
 * \param enable  scrolling two pages if `true`, one page otherwise.
 */
void vdp_set_hscroll_dual_page(bool enable);

// ---- VDP COMMANDs

/**
 * Tests if a VDP command is running.
 *
 * \return  `true` is a VDP command is running, `false` otherwise.
 *
 * \sa vdp_cmd_execute()
 */
inline bool vdp_cmd_is_running(void) {
  return (vdp_get_status(2) & 1);
}

/**
 * Wait for the VDP command to finish.
 *
 * \sa vdp_cmd_execute()
 */
inline void vdp_cmd_await(void) {
  while (vdp_cmd_is_running())
    ;
}

/**
 * Unristricts/Restricts availability of the VDP command for some screen modes.
 *
 * \param enable  If `true`, enables the VDP command for all screen modes. \n
 *                Otherwise, enables the VDP command only for GRAPHIC 4 to 7.
 *
 * \sa vdp_cmd_execute()
 * \sa vdp_set_screen_mode()
 */
void vdp_cmd_set_unrestricted(bool enable);

/**
 * Enumeration of logical operation codes for VDP commands.
 *
 * \sa vdp_cmd_execute()
 */
enum vdp_cmd_logop {
  /** Logical operation code "IMP" : destination ← source */
  VDP_CMD_IMP  = 0x00,
  /** Logical operation code "AND" : destination ← destination & source */
  VDP_CMD_AND  = 0x01,
  /** Logical operation code "OR" : destination ← destination | source */
  VDP_CMD_OR   = 0x02,
  /** Logical operation code "EOR" : destination ← destination ^ source */
  VDP_CMD_EOR  = 0x03,
  /** (synonim for `VDP_CMD_EOR`) */
  VDP_CMD_XOR  = VDP_CMD_EOR,
  /** Logical operation code "NOT" : destination ← ~source */
  VDP_CMD_NOT  = 0x04,
  /** Logical operation code "TIMP" : destination ← source , if source ≠ 0 */
  VDP_CMD_TIMP = 0x08,
  /** Logical operation code "TAND" : destination ← destination & source , if source ≠ 0 */
  VDP_CMD_TAND = 0x09,
  /** Logical operation code "TOR" : destination ← destination | source , if source ≠ 0 */
  VDP_CMD_TOR  = 0x0a,
  /** Logical operation code "TEOR" : destination ← destination ^ source , if source ≠ 0 */
  VDP_CMD_TEOR = 0x0b,
  /** (synonim for `VDP_CMD_TEOR`) */
  VDP_CMD_TXOR = VDP_CMD_TEOR,
  /** Logical operation code "TNOT" : destination ← ~source , if source ≠ 0 */
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
 * to the `struct vdp_cmd` object pointed by the 1st argument `c`.
 *
 * To set parameters, you may use the following inline functions:
 * - vdp_cmd_set_SX()
 * - vdp_cmd_set_SY()
 * - vdp_cmd_set_DX()
 * - vdp_cmd_set_DY()
 * - vdp_cmd_set_NX()
 * - vdp_cmd_set_NY()
 * - vdp_cmd_set_CLR()
 * - vdp_cmd_set_ARG()
 * - vdp_cmd_set_logop()
 *
 * After calling this function, if you want to wait for the VDP command to
 * finish, call vdp_cmd_await().
 *
 * \param c       pointer to `struct vdp_cmd`.
 * \param opcode  an operation code.
 *
 * \note
 * Currently, only the following operations are supported:
 * - VDP_CMD_LMMV
 * - VDP_CMD_LMMM
 * - VDP_CMD_HMMV
 * - VDP_CMD_HMMM
 *
 * \sa vdp_cmd_await()
 * \sa vdp_cmd_set_SX()
 * \sa vdp_cmd_set_SY()
 * \sa vdp_cmd_set_DX()
 * \sa vdp_cmd_set_DY()
 * \sa vdp_cmd_set_NX()
 * \sa vdp_cmd_set_NY()
 * \sa vdp_cmd_set_CLR()
 * \sa vdp_cmd_set_ARG()
 * \sa vdp_cmd_set_logop()
 */
void vdp_cmd_execute(const struct vdp_cmd * c, enum vdp_cmd_op opcode);

/**
 * Executes VDP command "LMMV" (fills rectangular area w/ logical operation).
 *
 * This function is equivalent to the following code:
 * ~~~
 * struct vdp_cmd c;
 * vdp_cmd_set_DX(&c, x);
 * vdp_cmd_set_DY(&c, y);
 * vdp_cmd_set_NX(&c, w);
 * vdp_cmd_set_NY(&c, h);
 * vdp_cmd_set_CLR(&c, color);
 * vdp_cmd_set_ARG(&c, 0);
 * vdp_cmd_set_logop(&c, logop);
 * vdp_cmd_execute(&c, VDP_CMD_LMMV);
 * ~~~
 *
 * \param x      x-coordinate of the top-left corner of the rectangular area.
 * \param y      y-coordinate of the top-left corner of the rectangular area.
 * \param w      width of the rectangular area.
 * \param h      height of the rectangular area.
 * \param color  color code.
 * \param logop  logical operation code.
 *
 * \sa vdp_cmd_execute()
 */
void vdp_cmd_execute_LMMV(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                          uint8_t color, enum vdp_cmd_logop logop);

/**
 * Executes VDP command "LMMM" (copy rectangular area w/ logical operation).
 *
 * This function is equivalent to the following code:
 * ~~~
 * struct vdp_cmd c;
 * vdp_cmd_set_SX(&c, x);
 * vdp_cmd_set_SY(&c, y);
 * vdp_cmd_set_DX(&c, x2);
 * vdp_cmd_set_DY(&c, y2);
 * vdp_cmd_set_NX(&c, w);
 * vdp_cmd_set_NY(&c, h);
 * vdp_cmd_set_ARG(&c, 0);
 * vdp_cmd_set_logop(&c, logop);
 * vdp_cmd_execute(&c, VDP_CMD_LMMM);
 * ~~~
 *
 * \param x      x-coordinate of the top-left corner of the source rectangular area.
 * \param y      y-coordinate of the top-left corner of the source rectangular area.
 * \param x2     x-coordinate of the top-left corner of the destination rectangular area.
 * \param y2     y-coordinate of the top-left corner of the destination rectangular area.
 * \param w      width of the rectangular area.
 * \param h      height of the rectangular area.
 * \param logop  logical operation code.
 *
 * \sa vdp_cmd_execute()
 */
void vdp_cmd_execute_LMMM(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                          uint16_t x2, uint16_t y2, enum vdp_cmd_logop logop);

/**
 * Executes VDP command "HMMV" (fills rectangular area).
 *
 * This function is equivalent to the following code:
 * ~~~
 * struct vdp_cmd c;
 * vdp_cmd_set_DX(&c, x);
 * vdp_cmd_set_DY(&c, y);
 * vdp_cmd_set_NX(&c, w);
 * vdp_cmd_set_NY(&c, h);
 * vdp_cmd_set_CLR(&c, color);
 * vdp_cmd_set_ARG(&c, 0);
 * vdp_cmd_set_logop(&c, 0);
 * vdp_cmd_execute(&c, VDP_CMD_HMMV);
 * ~~~
 *
 * \param x      x-coordinate of the top-left corner of the rectangular area.
 * \param y      y-coordinate of the top-left corner of the rectangular area.
 * \param w      width of the rectangular area.
 * \param h      height of the rectangular area.
 * \param color  color code.
 *
 * \sa vdp_cmd_execute()
 */
void vdp_cmd_execute_HMMV(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                          uint8_t color);

/**
 * Executes VDP command "HMMM" (copy rectangular area).
 *
 * This function is equivalent to the following code:
 * ~~~
 * struct vdp_cmd c;
 * vdp_cmd_set_SX(&c, x);
 * vdp_cmd_set_SY(&c, y);
 * vdp_cmd_set_DX(&c, x2);
 * vdp_cmd_set_DY(&c, y2);
 * vdp_cmd_set_NX(&c, w);
 * vdp_cmd_set_NY(&c, h);
 * vdp_cmd_set_ARG(&c, 0);
 * vdp_cmd_set_logop(&c, 0);
 * vdp_cmd_execute(&c, VDP_CMD_HMMM);
 * ~~~
 *
 * \param x      x-coordinate of the top-left corner of the source rectangular area.
 * \param y      y-coordinate of the top-left corner of the source rectangular area.
 * \param x2     x-coordinate of the top-left corner of the destination rectangular area.
 * \param y2     y-coordinate of the top-left corner of the destination rectangular area.
 * \param w      width of the rectangular area.
 * \param h      height of the rectangular area.
 *
 * \sa vdp_cmd_execute()
 */
void vdp_cmd_execute_HMMM(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                          uint16_t x2, uint16_t y2);


#endif
