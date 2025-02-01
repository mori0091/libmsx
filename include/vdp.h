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
 * \file vdp.h
 * \brief Data types and functions for VDP (Video Display Proccessor) access.
 * \ingroup VDP
 */
/**
 * \defgroup VDP VDP - Video Display Processor
 * `#include <vdp.h>`
 */

#pragma once

#ifndef VDP_H
#define VDP_H

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#include "io.h"
#include "vmem.h"
#include "vdp_unsafe.h"

#include "bios.h"
#include "workarea.h"

// ---- VDP status register
/**
 * \defgroup VDP_STATUS_REGISTER Status registers
 * \ingroup VDP
 * Read from status registers.
 * @{
 */

/**
 * `MSX` Read from a VDP status register.
 *
 * \param reg  VDP status register number.
 *             - `MSX` : `0`
 *             - `MSX2`: `0`..`9`
 *             - If any other value was specified, behaviour is undefined.
 * \return     Value of the VDP status register.
 *             - If `reg != 0`, returns the value of the status register #`reg`
 *             - If `reg == 0`, returns the value of `STATFL` instead of
 *               reading the status register #0 (S#0).
 *
 * \note
 * The MSX SYSTEM interrupt routine reads S#0 to see if an interrupt due to
 * VSYNC has occurred. Also, reading S#0 resets bit #7 (VSYNC interrupt flag) of
 * S#0, so the latest value read from S#0 is stored in `STATFL` for other uses.
 * Therefore, this function reads `STATFL` instead of S#0 if `reg == 0`.
 *
 * \sa STATFL
 */
uint8_t vdp_get_status(uint8_t reg);

/** @} */

// ---- VDP control registers
/**
 * \defgroup VDP_CONTROL_REGISTER Control registers
 * \ingroup VDP
 * Write to control registers.
 * @{
 */

/**
 * `MSX` Write to a VDP control register.
 *
 * \param reg  VDP control register number.
 * \param x    Value to be written.
 */
void vdp_set_control(uint8_t reg, uint8_t x);

/**
 * `MSX2` Write to a series of VDP control registers.
 *
 * \param reg  First number of the VDP control register.
 * \param src  Pointer to the beginning of the value to be written.
 * \param len  Number of VDP control registers and values.
 */
void vdp_write_control(uint8_t reg, void* src, uint8_t len);

/** @} */

// ---- VDP color register
/**
 * \defgroup VDP_COLOR_REGISTER Color registers
 * \ingroup VDP
 * Text and background color in TEXT 1 and 2 mode, and Backdrop color.
 * @{
 */

/**
 * `MSX` Set VDP color register.
 *
 * \param c  value to be set to VDP color register (R#7)
 *
 * \note
 * In TEXT 1 and TEXT 2 mode:
 * - bit #7..#4 : foreground color of text. (`0`..`15`)
 * - bit #3..#0 : background color of text. (`0`..`15`)
 *
 * \note
 * In MULTI COLOR mode:
 * - bit #7..#4 : (unused)
 * - bit #3..#0 : color of the backdrop plane. (`0`..`15`)
 * - Color of patterns are given by the **pattern generator table**.
 *
 * \note
 * In GRAPHIC 1..3 mode:
 * - bit #7..#4 : (unused)
 * - bit #3..#0 : color of the backdrop plane. (`0`..`15`)
 * - Color of patterns are given by the **color table**.
 *
 * \note
 * In GRAPHIC 4..6 mode:
 * - bit #7..#4 : (unused)
 * - bit #3..#0 : color of the backdrop plane. (`0`..`15`)
 * - Color of pixels are given by the **pattern name table**.
 *
 * \note
 * In GRAPHIC 7 mode (default):
 * - bit #7..#0 : color of the backdrop plane. (`0`..`255`)
 * - Color of pixels are given by the **pattern name table**.
 *
 * \note
 * In GRAPHIC 7 mode (YJK):
 * - bit #7..#4 : (unused)
 * - bit #3..#0 : color of the backdrop plane. (`0`..`15`)
 * - Color of pixels are given by the **pattern name table**.
 *
 * \note
 * In GRAPHIC 7 mode (YJK/RGB mixed):
 * - bit #7..#4 : (unused)
 * - bit #3..#0 : color of the backdrop plane. (`0`..`15`)
 * - Color of pixels are given by the **pattern name table**.
 */
void vdp_set_color(uint8_t c);  // set color register R#7

/** @} */

// ---- VDP palette registers
/**
 * \defgroup VDP_PALETTE_REGISTER Palette registers
 * \ingroup VDP
 * Color palettes.
 * @{
 */

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
 * `MSX2` Write to a VDP palette register.
 *
 * \param idx      color palette number. (0..15)
 * \param palette  color palette value.
 *
 * \sa RGB()
 */
void vdp_set_palette(uint8_t idx, const palette_t palette);

/**
 * `MSX2` Write to a series of VDP palette registers.
 *
 * \param palettes  16 color palette values.
 *
 * \sa RGB()
 */
void vdp_write_palette(const palette_t palettes[16]);

/** @} */

// ---- VDP display setting
/**
 * \defgroup VDP_MODE_REGISTER Mode registers
 * \ingroup VDP
 * Screen / Sprite mode setting.
 * @{
 */

/**
 * Enumeration of VDP screen modes.
 * \sa vdp_set_screen_mode()
 */
enum vdp_screen_mode {
  /** `MSX` GRAPHIC 1 (SCREEN 1) */
  VDP_SCREEN_MODE_GRAPHIC_1   = 0, // SCREEN 1
  /** `MSX` TEXT 1 (SCREEN 0, WIDTH 40)*/
  VDP_SCREEN_MODE_TEXT_1      = 1, // SCREEN 0: WIDTH 40
  /** `MSX` MULTI COLOR (SCREEN 3) */
  VDP_SCREEN_MODE_MULTI_COLOR = 2, // SCREEN 3
  /** `MSX` GRAPHIC 2 (SCREEN 2) */
  VDP_SCREEN_MODE_GRAPHIC_2   = 3, // SCREEN 2
  /** `MSX2` GRAPHIC 3 (SCREEN 4) */
  VDP_SCREEN_MODE_GRAPHIC_3   = 4, // SCREEN 4
  /** `MSX2` TEXT 2 (SCREEN 0, WIDTH 80)*/
  VDP_SCREEN_MODE_TEXT_2      = 5, // SCREEN 0: WIDTH 80
  /** `MSX2` GRAPHIC 4 (SCREEN 5) */
  VDP_SCREEN_MODE_GRAPHIC_4   = 6, // SCREEN 5
  /** `MSX2` GRAPHIC 5 (SCREEN 6) */
  VDP_SCREEN_MODE_GRAPHIC_5   = 7, // SCREEN 6
  /** `MSX2` GRAPHIC 6 (SCREEN 7) */
  VDP_SCREEN_MODE_GRAPHIC_6   = 8, // SCREEN 7
  /** `MSX2` GRAPHIC 7 (SCREEN 8) */
  VDP_SCREEN_MODE_GRAPHIC_7   = 9, // SCREEN 8
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
 * `MSX` Set VDP screen mode.
 *
 * | `enum vdp_screen_mode`        | VDP Screen mode   | BASIC Screen mode  | Sprite mode | compatibility |
 * | ----------------------------- | ----------------- | ------------------ | ----------- | ------------- |
 * | `VDP_SCREEN_MODE_TEXT_1`      | TEXT 1            | SCREEN 0, WIDTH 40 | (no sprite) | `MSX`         |
 * | `VDP_SCREEN_MODE_GRAPHIC_1`   | GRAPHIC 1         | SCREEN 1           | mode 1      | `MSX`         |
 * | `VDP_SCREEN_MODE_GRAPHIC_2`   | GRAPHIC 2         | SCREEN 2           | mode 1      | `MSX`         |
 * | `VDP_SCREEN_MODE_MULTI_COLOR` | MULTI COLOR       | SCREEN 3           | mode 1      | `MSX`         |
 * | `VDP_SCREEN_MODE_TEXT_2`      | TEXT 2            | SCREEN 0, WIDTH 80 | (no sprite) | `MSX2`        |
 * | `VDP_SCREEN_MODE_GRAPHIC_3`   | GRAPHIC 3         | SCREEN 4           | mode 2      | `MSX2`        |
 * | `VDP_SCREEN_MODE_GRAPHIC_4`   | GRAPHIC 4         | SCREEN 5           | mode 2      | `MSX2`        |
 * | `VDP_SCREEN_MODE_GRAPHIC_5`   | GRAPHIC 5         | SCREEN 6           | mode 2      | `MSX2`        |
 * | `VDP_SCREEN_MODE_GRAPHIC_6`   | GRAPHIC 6         | SCREEN 7           | mode 2      | `MSX2`        |
 * | `VDP_SCREEN_MODE_GRAPHIC_7`   | GRAPHIC 7         | SCREEN 8           | mode 2      | `MSX2`        |
 * | `VDP_SCREEN_MODE_GRAPHIC_7`   | GRAPHIC 7 YJK/RGB | SCREEN 10 or 11    | mode 2      | `MSX2+`       |
 * | `VDP_SCREEN_MODE_GRAPHIC_7`   | GRAPHIC 7 YJK     | SCREEN 12          | mode 2      | `MSX2+`       |
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
 *   SCREEN 10, 11, or 12), **YJK/YAE bits** shall be set/reset by calling
 *   vdp_set_yjk_mode().
 * - etc.
 *
 * \sa vdp_set_image_table()
 * \sa vdp_set_pattern_table()
 * \sa vdp_set_color_table()
 * \sa vdp_set_screen_lines()
 * \sa vdp_set_yjk_mode()
 * \sa vdp_set_sprite_pattern_table()
 * \sa vdp_set_sprite_attribute_table()
 * \sa vdp_set_sprite_size()
 * \sa vdp_get_sprite_mode()
 */
void vdp_set_screen_mode(enum vdp_screen_mode mode);

/**
 * `MSX2` Set number of visible lines.
 *
 * \param lines  `VDP_SCREEN_LINES_192` for 192 lines, or \n
 *               `VDP_SCREEN_LINES_212` for 212 lines.
 */
void vdp_set_screen_lines(enum vdp_screen_lines lines);

/**
 * `MSX` Show / hide screen.
 *
 * \param visible  show screen if `true`, hide otherwise.
 */
void vdp_set_visible(bool visible);

/**
 * `MSX` Get current sprite mode.
 *
 * The sprite mode will be determined based on the current screen mode.
 *
 * | `enum vdp_screen_mode`        | VDP Screen mode   | BASIC Screen mode  | Sprite mode | compatibility |
 * | ----------------------------- | ----------------- | ------------------ | ----------- | ------------- |
 * | `VDP_SCREEN_MODE_TEXT_1`      | TEXT 1            | SCREEN 0, WIDTH 40 | (no sprite) | `MSX`         |
 * | `VDP_SCREEN_MODE_GRAPHIC_1`   | GRAPHIC 1         | SCREEN 1           | **mode 1**  | `MSX`         |
 * | `VDP_SCREEN_MODE_GRAPHIC_2`   | GRAPHIC 2         | SCREEN 2           | **mode 1**  | `MSX`         |
 * | `VDP_SCREEN_MODE_MULTI_COLOR` | MULTI COLOR       | SCREEN 3           | **mode 1**  | `MSX`         |
 * | `VDP_SCREEN_MODE_TEXT_2`      | TEXT 2            | SCREEN 0, WIDTH 80 | (no sprite) | `MSX2`        |
 * | `VDP_SCREEN_MODE_GRAPHIC_3`   | GRAPHIC 3         | SCREEN 4           | **mode 2**  | `MSX2`        |
 * | `VDP_SCREEN_MODE_GRAPHIC_4`   | GRAPHIC 4         | SCREEN 5           | **mode 2**  | `MSX2`        |
 * | `VDP_SCREEN_MODE_GRAPHIC_5`   | GRAPHIC 5         | SCREEN 6           | **mode 2**  | `MSX2`        |
 * | `VDP_SCREEN_MODE_GRAPHIC_6`   | GRAPHIC 6         | SCREEN 7           | **mode 2**  | `MSX2`        |
 * | `VDP_SCREEN_MODE_GRAPHIC_7`   | GRAPHIC 7         | SCREEN 8           | **mode 2**  | `MSX2`        |
 * | `VDP_SCREEN_MODE_GRAPHIC_7`   | GRAPHIC 7 YJK/RGB | SCREEN 10 or 11    | **mode 2**  | `MSX2+`       |
 * | `VDP_SCREEN_MODE_GRAPHIC_7`   | GRAPHIC 7 YJK     | SCREEN 12          | **mode 2**  | `MSX2+`       |
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
 * `MSX` Set sprite size.
 *
 * \param size  sprite size.
 */
void vdp_set_sprite_size(enum vdp_sprite_size size);

/**
 * `MSX2` Show / hide sprites.
 *
 * \param visible  show sprites if `true`, hide otherwise.
 */
void vdp_set_sprite_visible(bool visible);

/** @} */

/**
 * \defgroup VDP_TABLE_BASE_ADDRESS_REGISTER Table base address registers
 * \ingroup VDP
 * VRAM memory map setting.
 * @{
 */

/**
 * `MSX` Set VRAM address of the pattern name table.
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
 * `MSX` Set VRAM address of the pattern generator table.
 *
 * \param table  VRAM address of the pattern generator table.
 *
 * \note
 * In bitmap graphic mode (GRAPHIC 4 to 7), this function need not be called.
 */
void vdp_set_pattern_table(vmemptr_t table);

/**
 * `MSX` Set VRAM address of the color table.
 *
 * \param table  VRAM address of the color table.
 *
 * \note
 * In bitmap graphic mode (GRAPHIC 4 to 7), this function need not be called.
 *
 * \note
 * In TEXT 1 mode, this function need not be called.
 *
 * \note
 * In TEXT 2 mode, this function is used to set the **blink table**.
 */
void vdp_set_color_table(vmemptr_t table);

/**
 * `MSX` Set VRAM address of the sprite pattern generator table.
 *
 * \param table  VRAM address of the sprite pattern generator table.
 *
 * \note
 * In text mode (TEXT 1 or TEXT 2), this function need not be called.
 */
void vdp_set_sprite_pattern_table(vmemptr_t table);

/**
 * `MSX` Set VRAM address of the sprite attribute table.
 *
 * \param table  VRAM address of the sprite attribute table.
 *
 * \note
 * In sprite mode 2, there is one more table called "sprite color table".
 * \note
 * The sprite color table is (implicitlly) placed at `SAT - 0x0200` ; where
 * `SAT` is the base address of sprite attribute table.
 *
 * \note
 * In text mode (TEXT 1 or TEXT 2), this function need not be called.
 */
void vdp_set_sprite_attribute_table(vmemptr_t table);

/** @} */

/**
 * \defgroup VDP_DISPLAY_REGISTER Display registers
 * \ingroup VDP
 * Dispalay area adjust / Hardware scroll.
 * @{
 */

/**
 * `MSX2` Set VDP display adjust register.
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
 * `MSX2` Set VDP vertical display offset register.
 *
 * \param y  vertical offset. (0..255)
 */
void vdp_set_vscroll(uint8_t y);

/**
 * `MSX2+` Set VDP horizontal display offset register.
 *
 * \param x  horizontal offset (0..511)
 */
void vdp_set_hscroll(uint16_t x);

/**
 * `MSX2+` Enables/Disables the screen mask for the leftmost 8 pixels.
 *
 * \param enable  Hide the leftmost 8 pixels if `true`, show otherwise.
 */
void vdp_set_hscroll_mask(bool enable);

/**
 * `MSX2+` Enable/disable horizontal scrolling for two pages.
 *
 * \param enable  scrolling two pages if `true`, one page otherwise.
 */
void vdp_set_hscroll_dual_page(bool enable);

/**
 * `MSX2+` Enumeration of V9958 VDP's color space.
 *
 * `VDP_RGB` can be used to reset to default color space for each screen mode.
 *
 * `VDP_YJK` and `VDP_YJK_RGB` are supposed to use with GRAPHIC 7 screen mode,
 * and not with any other screen mode.
 *
 * \sa vdp_set_yjk_mode()
 */
enum vdp_yjk_mode {
  /**
   * Default color space of the V9958 (same as the V9938).
   *
   * In case of GRAPHIC 7 screen mode:
   * - 256 colors
   *   - 3-bits per pixel of R component (`0`..`7`)
   *   - 3-bits per pixel of G component (`0`..`7`)
   *   - 2-bits per pixel of B component (`0`..`3`)
   * - Pre-defined 16 colors for sprites.
   * - No color palette.
   *
   * Any other screen mode:
   * - According to colors for each screen mode.
   */
  VDP_RGB     = 0x00,
  /**
   * YJK mode.
   *
   * In case of GRAPHIC 7 screen mode:
   * - 19,268 colors
   *   - 5-bits per pixel of Y component (`0`..`31`)
   *   - 6-bits per 4-pixels of J component (`-32`..`+31`)
   *   - 6-bits per 4-pixels of K component (`-32`..`+31`)
   * - 16 colors out of 512 colors (color palette) for sprites.
   *
   * Any other screen mode:
   * - Not supposed to use with.
   */
  VDP_YJK     = 0x08,
  /**
   * YJK / RGB mixed mode.
   *
   * In case of GRAPHIC 7 screen mode:
   * - 12,499 colors
   *   - 4-bits per pixel of Y component (`0`..`15`)
   *   - 6-bits per 4-pixels of J component (`-32`..`+31`)
   *   - 6-bits per 4-pixels of K component (`-32`..`+31`)
   * - or 16 colors out of 512 colors (color palette).
   * - 16 colors out of 512 colors (color palette) for sprites.
   *
   * Any other screen mode:
   * - Not supposed to use with.
   */
  VDP_YJK_RGB = 0x18,
};

/**
 * `MSX2+` Set V9958 VDP's color space to RGB, YJK, or YJK/RGB.
 *
 * Set / Reset YJK/YAE bit of V9958 VDP register R#25.
 *
 * Affects GRAPHIC 7 screen mode color space and pixel format.
 *
 * `VDP_RGB` can be used to reset to default color space for each screen mode.
 *
 * `VDP_YJK` and `VDP_YJK_RGB` are supposed to use with GRAPHIC 7 screen mode,
 * and not with any other screen mode.
 *
 * \param yjk    color space
 */
void vdp_set_yjk_mode(enum vdp_yjk_mode yjk);

/** @} */

// ---- VDP COMMANDs
/**
 * \defgroup VDP_COMMANDS_HIGH VDP commands (High level APIs)
 * \ingroup VDP
 * Execute, Stop, and Await VDP commands.
 * @{
 */

/**
 * `MSX2` Tests if a VDP command is running.
 *
 * \return  `true` is a VDP command is running, `false` otherwise.
 *
 * \sa vdp_cmd_execute()
 */
inline bool vdp_cmd_is_running(void) {
  return (vdp_get_status(2) & 1);
}

/**
 * `MSX2` Wait for the VDP command to finish.
 *
 * \sa vdp_cmd_execute()
 */
inline void vdp_cmd_await(void) {
  while (vdp_cmd_is_running())
    ;
}

/**
 * `MSX2+` Unristricts/Restricts availability of the VDP command for some screen modes.
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
 * Enumeration of directions of rectangular operation for VDP commands.
 *
 * \sa vdp_cmd_execute_LMCM()
 * \sa vdp_cmd_execute_LMMC()
 * \sa vdp_cmd_execute_HMMC()
 */
enum vdp_cmd_dir {
  VDP_CMD_LRTB = (0 << 2),      ///< Left to Right, Top to Bottom.
  VDP_CMD_RLTB = (1 << 2),      ///< Right to Left, Top to Bottom.
  VDP_CMD_LRBT = (2 << 2),      ///< Left to Right, Bottom to Top.
  VDP_CMD_RLBT = (3 << 2),      ///< Right to Left, Bottom to Top.
};

/**
 * `MSX2` Executes VDP command "STOP" (stop the running VDP command).
 */
inline void vdp_cmd_stop(void) {
  vdp_set_control(46, 0);
}

/**
 * `MSX2` Executes VDP command "POINT" (Read color code at the given point).
 *
 * \param sx    x-coordinate (0..511)
 * \param sy    y-coordinate (0..1023)
 * \return      color code at (sx, sy)
 */
uint8_t vdp_cmd_execute_POINT(uint16_t sx, uint16_t sy);

/**
 * `MSX2` Executes VDP command "PSET" (Write color code at the given point).
 *
 * \param dx    x-coordinate (0..511)
 * \param dy    y-coordinate (0..1023)
 * \param color color code
 * \param logop logical operation code
 */
void vdp_cmd_execute_PSET(uint16_t dx, uint16_t dy,
                          uint8_t color,
                          enum vdp_cmd_logop logop);

/**
 * `MSX2` Executes VDP command "SRCH" (Search color code from the given point towards left/right).
 *
 * \param ret   pointer to a variable where the X coordinate of the detected point is stored.
 * \param sx    x-coordinate (0..511)
 * \param sy    y-coordinate (0..1023)
 * \param color color code
 * \param arg   argument register value
 *              - bit #2 : (0) search towards right, (1) search towards left.
 *              - bit #1 : (0) search the specified color, (1) search except for the specified color.
 *
 * \return `true` if found, `false` otherwise.
 */
bool vdp_cmd_execute_SRCH(uint16_t * ret,
                          uint16_t sx, uint16_t sy,
                          uint8_t color,
                          uint8_t arg);

/**
 * `MSX2` Executes VDP command "LINE" (Draw line w/ logical operation).
 *
 * \param x1    x-coordinate of the 1st end point (0..511)
 * \param y1    y-coordinate of the 1st end point (0..1023)
 * \param x2    x-coordinate of the 2nd end point (0..511)
 * \param y2    y-coordinate of the 2nd end point (0..1023)
 * \param color color code
 * \param logop logical operation code
 */
void vdp_cmd_execute_LINE(uint16_t x1, uint16_t y1,
                          uint16_t x2, uint16_t y2,
                          uint8_t color,
                          enum vdp_cmd_logop logop);


/**
 * `MSX2` Reads a value from VDP.
 *
 * \param ret   pointer to a variable where the value read from VDP is stored.
 *
 * \return      `true` if succeeded, otherwise `false`.
 *
 * \sa vdp_cmd_execute_LMCM()
 */
bool vdp_cmd_read(uint8_t * ret);

/**
 * `MSX2` Executes VDP command "LMCM" (VRAM to CPU logical transfer).
 *
 * Set VDP to read out **pixel** values in the rectangular area of VRAM.
 *
 * The **pixel** values (i.e. color code) can then be read by calling
 * vdp_cmd_read().
 *
 * After this function, vdp_cmd_read() should be called repeatedly, or
 * vdp_cmd_stop() should be called if the read operation is to be canceled.
 *
 * ~~~ c
 * // Read out **pixel** values in the rectangular area (0,0)..(7,7) into `pixels[]`.
 *
 * #define WIDTH        (8) // Width 8-pixels
 * #define HEIGHT       (8) // Height 8-pixels
 *
 * uint8_t pixels[8*8];
 *
 * vdp_cmd_execute_LMCM(0, 0, WIDTH, HEIGHT, VDP_CMD_LRTB);
 * uint8_t * p = &pixels[0];
 * for (uint8_t n = sizeof(pixels); n--; ) {
 *   if (!vdp_cmd_read(p++)) {
 *     // failed to read (the command maight be terminated)
 *     break;
 *   }
 * }
 * ~~~
 *
 * \param x     x-coordinate of the starting point (0..511)
 * \param y     y-coordinate of the starting point (0..1023)
 * \param w     width (1..512)
 * \param h     height (1..1024)
 * \param dir   direction
 *              - VDP_CMD_LRTB : Left to right, top to bottom
 *              - VDP_CMD_RLTB : Right to left, top to bottom
 *              - VDP_CMD_LRBT : Left to right, bottom to top
 *              - VDP_CMD_RLBT : Right to left, bottom to top
 *
 * \sa vdp_cmd_read()
 * \sa vdp_cmd_stop()
 */
void vdp_cmd_execute_LMCM(uint16_t x, uint16_t y,
                          uint16_t w, uint16_t h,
                          enum vdp_cmd_dir dir);

/**
 * `MSX2` Writes a value to VDP.
 *
 * \param val   the value to be written to VDP.
 *
 * \return      `true` if succeeded, otherwise `false`.
 *
 * \sa vdp_cmd_execute_LMMC()
 * \sa vdp_cmd_execute_HMMC()
 */
bool vdp_cmd_write(const uint8_t val);

/**
 * `MSX2` Executes VDP command "LMMC" (CPU to VRAM logical transfer).
 *
 * Set VDP to write out **pixel** values in the rectangular area of VRAM.
 *
 * The **pixel** values (i.e. color code) can then be written by calling
 * vdp_cmd_write().
 *
 * After this function, vdp_cmd_write() should be called repeatedly, or
 * vdp_cmd_stop() should be called if the write operation is to be canceled.
 *
 * ~~~ c
 * // Write out **pixel** values in `pixels[]` into the rectangular area (0,0)..(7,7).
 *
 * #define WIDTH        (8) // Width 8-pixels
 * #define HEIGHT       (8) // Height 8-pixels
 *
 * uint8_t pixels[WIDTH * HEIGHT] = { ... };
 *
 * vdp_cmd_execute_LMMC(0, 0, WIDTH, HEIGHT, VDP_CMD_LRTB, VDP_CMD_IMP);
 * uint8_t * p = &pixels[0];
 * for (uint8_t n = sizeof(pixels); n--; ) {
 *   if (!vdp_cmd_write(*p++)) {
 *     // failed to write (the command maight be terminated)
 *     break;
 *   }
 * }
 * ~~~
 *
 * \param x     x-coordinate of the starting point (0..511)
 * \param y     y-coordinate of the starting point (0..1023)
 * \param w     width (1..512)
 * \param h     height (1..1024)
 * \param dir   direction
 *              - VDP_CMD_LRTB : Left to right, top to bottom
 *              - VDP_CMD_RLTB : Right to left, top to bottom
 *              - VDP_CMD_LRBT : Left to right, bottom to top
 *              - VDP_CMD_RLBT : Right to left, bottom to top
 * \param logop logical operation code
 *
 * \sa vdp_cmd_write()
 * \sa vdp_cmd_stop()
 */
void vdp_cmd_execute_LMMC(uint16_t x, uint16_t y,
                          uint16_t w, uint16_t h,
                          enum vdp_cmd_dir dir,
                          enum vdp_cmd_logop logop);

/**
 * `MSX2` Executes VDP command "HMMC" (CPU to VRAM high speed transfer).
 *
 * Set VDP to write out **byte** values in the rectangular area of VRAM.
 *
 * The **byte** values can then be written by calling vdp_cmd_write().
 *
 * After this function, vdp_cmd_write() should be called repeatedly, or
 * vdp_cmd_stop() should be called if the write operation is to be canceled.
 *
 * ~~~ c
 * // Write out **byte** values in `bytes[]` into the rectangular area (0,0)..(7,7).
 *
 * #define WIDTH        (8) // Width 8-pixels
 * #define HEIGHT       (8) // Height 8-pixels
 * #define BPP          (4) // Number of bits per pixel
 *                          // - `4` for SCREEN 5 and 7,
 *                          // - `2` for SCREEN 6,
 *                          // - `8` for SCREEN 8.
 * #define PIX_PER_BYTE (8 / BPP)
 * #define NUM_BYTES  ((WIDTH + PIX_PER_BYTE - 1) / PIX_PER_BYTE * HEIGHT)
 *
 * uint8_t bytes[NUM_BYTES] = { ... };
 *
 * vdp_cmd_execute_HMMC(0, 0, WIDTH, HEIGHT, VDP_CMD_LRTB);
 * uint8_t * p = &bytes[0];
 * for (size_t n = sizeof(bytes); n--; ) {
 *   if (!vdp_cmd_write(*p++)) {
 *     // failed to write (the command maight be terminated)
 *     break;
 *   }
 * }
 * ~~~
 *
 * \param x     x-coordinate of the starting point (0..511)
 * \param y     y-coordinate of the starting point (0..1023)
 * \param w     width (1..512)
 * \param h     height (1..1024)
 * \param dir   direction
 *              - VDP_CMD_LRTB : Left to right, top to bottom
 *              - VDP_CMD_RLTB : Right to left, top to bottom
 *              - VDP_CMD_LRBT : Left to right, bottom to top
 *              - VDP_CMD_RLBT : Right to left, bottom to top
 *
 * \sa vdp_cmd_write()
 * \sa vdp_cmd_stop()
 *
 * \note
 * The parameter `x` and `w` must be aligned to byte address of VRAM.\n
 * In other words,
 * - LSB 1-bits are omitted for 4-bpp mode; 2 pixels per byte; GRAPHIC 4 and 6 (SCREEN 5 and 7),
 * - LSB 2-bits are omitted for 2-bpp mode; 4 pixels per byte; GRAPHIC 5 (SCREEN 6).
 * - No bits are omitted for 8-bpp mode   ; 1 pixels per byte; GRAPHIC 7 (SCREEN 8).
 */
void vdp_cmd_execute_HMMC(uint16_t x, uint16_t y,
                          uint16_t w, uint16_t h,
                          enum vdp_cmd_dir dir);

/**
 * `MSX2` Executes VDP command "LMMV" (fills rectangular area w/ logical operation).
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
 * `MSX2` Executes VDP command "LMMM" (copy rectangular area w/ logical operation).
 *
 * This function is equivalent to the following code, as long as the
 * intersection of the source and destination areas is not considered.
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
 * `MSX2` Executes VDP command "HMMV" (fills rectangular area).
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
 *
 * \note
 * The parameter `x` and `w` must be aligned to byte address of VRAM.\n
 * In other words,
 * - LSB 1-bits are omitted for 4-bpp mode; 2 pixels per byte; GRAPHIC 4 and 6 (SCREEN 5 and 7),
 * - LSB 2-bits are omitted for 2-bpp mode; 4 pixels per byte; GRAPHIC 5 (SCREEN 6).
 * - No bits are omitted for 8-bpp mode   ; 1 pixels per byte; GRAPHIC 7 (SCREEN 8).
 */
void vdp_cmd_execute_HMMV(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                          uint8_t color);

/**
 * `MSX2` Executes VDP command "HMMM" (copy rectangular area).
 *
 * This function is equivalent to the following code, as long as the
 * intersection of the source and destination areas is not considered.
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
 *
 * \note
 * The parameter `x`, `x2`, and `w` must be aligned to byte address of VRAM.\n
 * In other words,
 * - LSB 1-bits are omitted for 4-bpp mode; 2 pixels per byte; GRAPHIC 4 and 6 (SCREEN 5 and 7),
 * - LSB 2-bits are omitted for 2-bpp mode; 4 pixels per byte; GRAPHIC 5 (SCREEN 6).
 * - No bits are omitted for 8-bpp mode   ; 1 pixels per byte; GRAPHIC 7 (SCREEN 8).
 */
void vdp_cmd_execute_HMMM(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                          uint16_t x2, uint16_t y2);

/**
 * `MSX2` Executes VDP command "YMMM" (copy rectangular area from VRAM to VRAM vertically).
 *
 * This function is equivalent to the following code, as long as the
 * intersection of the source and destination areas is not considered.
 * ~~~
 * struct vdp_cmd c;
 * vdp_cmd_set_SY(&c, y);
 * vdp_cmd_set_DX(&c, x);
 * vdp_cmd_set_DY(&c, y2);
 * vdp_cmd_set_NY(&c, h);
 * vdp_cmd_set_ARG(&c, dir);
 * vdp_cmd_set_logop(&c, 0);
 * vdp_cmd_execute(&c, VDP_CMD_YMMM);
 * ~~~
 *
 * \param x      x-coordinate of the base point of the source / destination rectangular area.
 * \param y      y-coordinate of the base point of the source rectangular area.
 * \param y2     y-coordinate of the base point of the destination rectangular area.
 * \param h      height of the rectangular area.
 * \param dir    (0) move right side area, (4) move left side area.
 *
 * \sa vdp_cmd_execute()
 *
 * \note
 * The parameter `x` must be aligned to byte address of VRAM.\n
 * In other words,
 * - LSB 1-bits are omitted for 4-bpp mode; 2 pixels per byte; GRAPHIC 4 and 6 (SCREEN 5 and 7),
 * - LSB 2-bits are omitted for 2-bpp mode; 4 pixels per byte; GRAPHIC 5 (SCREEN 6).
 * - No bits are omitted for 8-bpp mode   ; 1 pixels per byte; GRAPHIC 7 (SCREEN 8).
 */
void vdp_cmd_execute_YMMM(uint16_t x, uint16_t y, uint16_t h, uint16_t y2, uint8_t dir);

/** @} */

/**
 * \defgroup VDP_COMMANDS_LOW VDP commands (Low level APIs)
 * \ingroup VDP
 * Write to command registers.
 * @{
 */

/**
 * Enumeration of operation codes for VDP commands.
 *
 * \sa vdp_cmd_execute()
 */
enum vdp_cmd_op {
  /**
   * Operation code "STOP" : Stop the running VDP command.
   * \sa vdp_cmd_stop()
   */
  VDP_CMD_STOP = 0x00,
  /**
   * Operation code "POINT" : Read color code at the given point.
   * \sa vdp_cmd_execute_POINT()
   */
  VDP_CMD_POINT = 0x40,
  /**
   * Operation code "PSET" : Write color code at the given point.
   * \sa vdp_cmd_execute_PSET()
   */
  VDP_CMD_PSET = 0x50,
  /**
   * Operation code "SRCH" : Search color code from the given point towards left/right.
   * \sa vdp_cmd_execute_SRCH()
   */
  VDP_CMD_SRCH = 0x60,
  /**
   * Operation code "LINE" : Draw line w/ logical operation.
   * \sa vdp_cmd_execute_LINE()
   */
  VDP_CMD_LINE = 0x70,
  /**
   * Operation code "LMMV" : Fill rectangular area w/ logical operation.
   * \sa vdp_cmd_execute_LMMV()
   */
  VDP_CMD_LMMV = 0x80,
  /**
   * Operation code "LMMM" : Copy rectangular area from VRAM to VRAM w/ logical operation.
   * \sa vdp_cmd_execute_LMMM()
   */
  VDP_CMD_LMMM = 0x90,
  /**
   * Operation code "LMCM" : VRAM to CPU logical transfer.
   * \sa vdp_cmd_execute_LMCM()
   */
  VDP_CMD_LMCM = 0xa0,
  /**
   * Operation code "LMMC" : CPU to VRAM logical transfer.
   * \sa vdp_cmd_execute_LMMC()
   */
  VDP_CMD_LMMC = 0xb0,
  /**
   * Operation code "HMMV" : Fill rectangular area.
   * \sa vdp_cmd_execute_HMMV()
   */
  VDP_CMD_HMMV = 0xc0,
  /**
   * Operation code "HMMM" : Copy rectangular area from VRAM to VRAM.
   * \sa vdp_cmd_execute_HMMM()
   */
  VDP_CMD_HMMM = 0xd0,
  /**
   * Operation code "YMMM" : Copy rectangular area from VRAM to VRAM vertically.
   * \sa vdp_cmd_execute_YMMM()
   */
  VDP_CMD_YMMM = 0xe0,
  /**
   * Operation code "HMMC" : CPU to VRAM high speed transfer.
   * \sa vdp_cmd_execute_HMMC()
   */
  VDP_CMD_HMMC = 0xf0,
};

/**
 * Parameters for VDP commands.
 *
 * \sa vdp_cmd_execute()
 */
struct vdp_cmd {
  uint8_t r32;            ///< SX (lo)
  uint8_t r33;            ///< SX (hi)
  uint8_t r34;            ///< SY (lo)
  uint8_t r35;            ///< SY (hi)
  uint8_t r36;            ///< DX (lo)
  uint8_t r37;            ///< DX (hi)
  uint8_t r38;            ///< DY (lo)
  uint8_t r39;            ///< DY (hi)
  uint8_t r40;            ///< NX (lo)
  uint8_t r41;            ///< NX (hi)
  uint8_t r42;            ///< NY (lo)
  uint8_t r43;            ///< NY (hi)
  uint8_t r44;            ///< CLR (2bpp * 4 pix, 4bpp * 2 pix, or 8bpp * 1 pix)
  uint8_t r45;            ///< ARG (0000b | DIY | DIX | 00b)
  uint8_t r46;            ///< CMR (op | logop)
};

/**
 * `MSX2` Set SX value (x-coordinate of the source point) for VDP commands.
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
 * `MSX2` Set SY value (y-coordinate of the source point) for VDP commands.
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
 * `MSX2` Set DX value (x-coordinate of the destination point) for VDP commands.
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
 * `MSX2` Set DY value (y-coordinate of the destination point) for VDP commands.
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
 * `MSX2` Set NX value (width) for VDP commands.
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
 * `MSX2` Set NY value (height) for VDP commands.
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
 * `MSX2` Set CLR value (color) for VDP commands.
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
 * `MSX2` Set ARG value (DIX, DIY, etc.) for VDP commands.
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
 * `MSX2` Set a logical operation code for VDP commands.
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
 * `MSX2` Executes a VDP command.
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

/** @} */

#endif
