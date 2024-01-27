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
 * \file screen.h
 * \brief SCREEN mode initialization function like MSX-BASIC.
 * \ingroup SCREEN
 */
/**
 * \defgroup SCREEN SCREEN mode initialization
 * `#include <screen.h>`
 *
 * Functions corresponding to MSX-BASIC's following SCREEN modes:
 * - `MSX`  screen0()     : `SCREEN 0: WIDTH 40` (TEXT 1 mdoe),
 * - `MSX`  screen1()     : `SCREEN 1: WIDTH 32` (GRAPHIC 1 mode),
 * - `MSX`  screen2()     : `SCREEN 2: WIDTH 32` (GRAPHIC 2 mode),
 * - `MSX`  screen2_PCG() : `SCREEN 2: WIDTH 32` (GRAPHIC 2 mode; PCG mode like as SCREEN 1).
 * - `MSX2` screen0_W80() : `SCREEN 0: WIDTH 80` (TEXT 2 mode),
 * - `MSX2` screen4()     : `SCREEN 4: WIDTH 32` (GRAPHIC 3 mode),
 * - `MSX2` screen4_PCG() : `SCREEN 4: WIDTH 32` (GRAPHIC 3 mode; PCG mode like as SCREEN 1).
 * - `MSX2` screen5()     : `SCREEN 5`           (GRAPHIC 4 mode; 256x212 pix, 4-bpp),
 * - `MSX2` screen6()     : `SCREEN 6`           (GRAPHIC 5 mode; 512x212 pix, 2-bpp),
 * - `MSX2` screen7()     : `SCREEN 7`           (GRAPHIC 6 mode; 512x212 pix, 4-bpp),
 * - `MSX2` screen8()     : `SCREEN 8`           (GRAPHIC 7 mode; 256x212 pix, 8-bpp RGB332),
 * - `MSX2+` screen10()   : `SCREEN 10`          (GRAPHIC 7 mode; 256x212 pix, 8-bpp YJK/RGB),
 * - `MSX2+` screen12()   : `SCREEN 12`          (GRAPHIC 7 mode; 256x212 pix, 8-bpp YJK),
 *
 * **All** above SCREEN modes supports:
 * - printf() and putchar() standard C library functions (`#include <stdio.h>`), and
 * - text utility functions of `libmsx` such as locate(), print() (`#include <text.h>`).
 *
 * \sa text.h
 */

#pragma once

#ifndef SCREEN_H_
#define SCREEN_H_

/**
 * \defgroup SCREEN_TEXT TEXT mode
 * \ingroup SCREEN
 * Initialization functions for TEXT 1 and 2 mode.
 * @{
 */

/**
 * `MSX` SCREEN 0: WIDTH 40 (TEXT 1 mode).
 *
 * - 40 columns x 24 lines text mode.
 * - No sprites.
 *
 * **VRAM memory map**
 * | VRAM address | size   | contents                        |
 * |--------------|--------|---------------------------------|
 * | 0x00000      | 0x03c0 | Pattern name table              |
 * | 0x00800      | 0x0800 | Pattern generator table         |
 *
 * \note
 * Patterns can be defined for each 256-character fonts (**pattern generator
 * table** in VRAM).
 */
void screen0(void);

/**
 * `MSX2` SCREEN 0: WIDTH 80 (TEXT 2 mode).
 *
 * - 80 columns x 24 lines text mode.
 * - No sprites.
 *
 * **VRAM memory map**
 * | VRAM address | size   | contents                        |
 * |--------------|--------|---------------------------------|
 * | 0x00000      | 0x0780 | Pattern name table              |
 * | 0x01000      | 0x0800 | Pattern generator table         |
 * | 0x00800      | 0x010e | Color table                     |
 *
 * **Color table** is used as **blink table** (1-bit * 80 columns * 24 lines).
 * Characters blinks if the corresponding bit of the color table is set,
 * according to the value of VDP control register R#12 and R#13.
 *
 * \note
 * Patterns can be defined for each 256-character fonts (**pattern generator
 * table** in VRAM).
 */
void screen0_W80(void);

/** @} */

/**
 * \defgroup SCREEN_PCG Tiled graphics mode
 * \ingroup SCREEN
 * Initialization functions for GRAPHIC 1, 2, and 3 mode.
 * @{
 */

/**
 * `MSX` SCREEN 1: WIDTH 32 (GRAPHIC 1 mode).
 *
 * - 32 columns x 24 lines tiled graphics mode.
 * - Sprite mode 1.
 *
 * **VRAM memory map**
 * | VRAM address | size   | contents                        |
 * |--------------|--------|---------------------------------|
 * | 0x00000      | 0x0800 | Pattern generator table         |
 * | 0x01800      | 0x0300 | Pattern name table              |
 * | 0x01B00      | 0x0080 | Sprite attribute table          |
 * | 0x02000      | 0x0020 | Color table                     |
 * | 0x03800      | 0x0300 | Sprite pattern generator table  |
 *
 * \note
 * Patterns can be defined for each 256-character fonts (**pattern generator
 * table** in VRAM).
 *
 * \note
 * Colors can be defined for each 8-character group of 256-character fonts
 * (**color table** in VRAM).
 */
void screen1(void);

/**
 * `MSX` SCREEN 2: WIDTH 32 (GRAPHIC 2 mode).
 *
 * - 32 columns x 24 lines tiled graphics mode.
 * - Sprite mode 1.
 *
 * **VRAM memory map**
 * | VRAM address | size   | contents                        |
 * |--------------|--------|---------------------------------|
 * | 0x00000      | 0x1800 | Pattern generator table         |
 * | 0x01800      | 0x0300 | Pattern name table              |
 * | 0x01B00      | 0x0080 | Sprite attribute table          |
 * | 0x02000      | 0x1800 | Color table                     |
 * | 0x03800      | 0x0300 | Sprite pattern generator table  |
 *
 * The **pattern name table** is initialized as follows and will not be changed
 * thereafter:
 * ~~~ c
 * uint8_t pattern_name_table[0x0300] = {
 *   0x00, 0x01, ..., 0xff,  // (0, 0) .. (31, 7) ; top of screen
 *   0x00, 0x01, ..., 0xff,  // (0, 8) .. (31,15) ; middle of screen
 *   0x00, 0x01, ..., 0xff,  // (0,16) .. (31,23) ; bottom of screen
 * };
 * ~~~
 *
 * The **pattern generator table** and **color table** are then used as the
 * canvas for the graphics screen. Thus, the characters in the text are also
 * drawn as images one by one. Thus, the text will be more colorful, but a
 * little slower.
 *
 * \note
 * In this mode, text is drawn as individual color text.
 * (i.e., text is drawn as a two-tone color graphic).
 */
void screen2(void);

/**
 * `MSX` SCREEN 2: WIDTH 32 (GRAPHIC 2 mode; PCG mode like as SCREEN 1).
 *
 * - 32 columns x 24 lines tiled graphics mode.
 * - Sprite mode 1.
 *
 * **VRAM memory map**
 * | VRAM address | size   | contents                        |
 * |--------------|--------|---------------------------------|
 * | 0x00000      | 0x1800 | Pattern generator table         |
 * | 0x01800      | 0x0300 | Pattern name table              |
 * | 0x01B00      | 0x0080 | Sprite attribute table          |
 * | 0x02000      | 0x1800 | Color table                     |
 * | 0x03800      | 0x0300 | Sprite pattern generator table  |
 *
 * \note
 * For each 256-character font, three patterns/colors can be defined for the
 * top, middle, and bottom of the screen (**pattern generator table** and
 * **color table** in VRAM).
 *
 * \note a.k.a. SCREEN 1.5
 */
void screen2_PCG(void);

/**
 * `MSX2` SCREEN 4: WIDTH 32 (GRAPHIC 3 mode).
 *
 * - 32 columns x 24 lines tiled graphics mode.
 * - Sprite mode 2.
 *
 * **VRAM memory map**
 * | VRAM address | size   | contents                        |
 * |--------------|--------|---------------------------------|
 * | 0x00000      | 0x1800 | Pattern generator table         |
 * | 0x01800      | 0x0300 | Pattern name table              |
 * | 0x01C00      | 0x0200 | Sprite color table              |
 * | 0x01E00      | 0x0080 | Sprite attribute table          |
 * | 0x02000      | 0x1800 | Color table                     |
 * | 0x03800      | 0x0300 | Sprite pattern generator table  |
 *
 * The **pattern name table** is initialized as follows and will not be changed
 * thereafter:
 * ~~~ c
 * uint8_t pattern_name_table[0x0300] = {
 *   0x00, 0x01, ..., 0xff,  // (0, 0) .. (31, 7) ; top of screen
 *   0x00, 0x01, ..., 0xff,  // (0, 8) .. (31,15) ; middle of screen
 *   0x00, 0x01, ..., 0xff,  // (0,16) .. (31,23) ; bottom of screen
 * };
 * ~~~
 *
 * The **pattern generator table** and **color table** are then used as the
 * canvas for the graphics screen. Thus, the characters in the text are also
 * drawn as images one by one. Thus, the text will be more colorful, but a
 * little slower.
 *
 * \note
 * In this mode, text is drawn as individual color text.
 * (i.e., text is drawn as a two-tone color graphic).
 */
void screen4(void);

/**
 * `MSX2` SCREEN 4: WIDTH 32 (GRAPHIC 3 mode; PCG mode like as SCREEN 1).
 *
 * - 32 columns x 24 lines tiled graphics mode.
 * - Sprite mode 2.
 *
 * **VRAM memory map**
 * | VRAM address | size   | contents                        |
 * |--------------|--------|---------------------------------|
 * | 0x00000      | 0x1800 | Pattern generator table         |
 * | 0x01800      | 0x0300 | Pattern name table              |
 * | 0x01C00      | 0x0200 | Sprite color table              |
 * | 0x01E00      | 0x0080 | Sprite attribute table          |
 * | 0x02000      | 0x1800 | Color table                     |
 * | 0x03800      | 0x0300 | Sprite pattern generator table  |
 *
 * \note
 * For each 256-character font, three patterns/colors can be defined for the
 * top, middle, and bottom of the screen (**pattern generator table** and
 * **color table** in VRAM).
 */
void screen4_PCG(void);

/** @} */

/**
 * \defgroup SCREEN_BITMAP Bitmap graphics mode
 * \ingroup SCREEN
 * Initialization functions for GRAPHIC 4, 5, 6, and 7 mode.
 * @{
 */

/**
 * `MSX2` SCREEN 5 (GRAPHIC 4 mode).
 *
 * - 256 x 212 pixel, 4-bpp graphics mode.
 * - 16 colors/pixel of 512 colors (RGB333).
 * - Sprite mode 2.
 *
 * **VRAM memory map**
 * | VRAM address | size   | contents                        |
 * |--------------|--------|---------------------------------|
 * | 0x00000      | 0x6A00 | Pattern name table              |
 * | 0x07400      | 0x0200 | Sprite color table              |
 * | 0x07600      | 0x0080 | Sprite attribute table          |
 * | 0x07800      | 0x0300 | Sprite pattern generator table  |
 *
 * \note
 * In this mode, text is drawn as individual color text.
 * (i.e., text is drawn as a two-tone color graphic).
 */
void screen5(void);

/**
 * `MSX2` SCREEN 6 (GRAPHIC 5 mode).
 *
 * - 512 x 212 pixel, 2-bpp graphics mode.
 * - 4 colors/pixel of 512 colors (RGB333).
 * - Sprite mode 2.
 *
 * **VRAM memory map**
 * | VRAM address | size   | contents                        |
 * |--------------|--------|---------------------------------|
 * | 0x00000      | 0x6A00 | Pattern name table              |
 * | 0x07400      | 0x0200 | Sprite color table              |
 * | 0x07600      | 0x0080 | Sprite attribute table          |
 * | 0x07800      | 0x0300 | Sprite pattern generator table  |
 *
 * \note
 * In this mode, text is drawn as individual color text.
 * (i.e., text is drawn as a two-tone color graphic).
 */
void screen6(void);

/**
 * `MSX2` SCREEN 7 (GRAPHIC 6 mode).
 *
 * - 512 x 212 pixels, 4-bpp graphics mode.
 * - 16 colors/pixel of 512 colors (RGB333).
 * - Sprite mode 2.
 *
 * **VRAM memory map**
 * | VRAM address | size   | contents                        |
 * |--------------|--------|---------------------------------|
 * | 0x00000      | 0xD400 | Pattern name table              |
 * | 0x0F000      | 0x0300 | Sprite pattern generator table  |
 * | 0x0F800      | 0x0200 | Sprite color table              |
 * | 0x0FA00      | 0x0080 | Sprite attribute table          |
 *
 * \note
 * In this mode, text is drawn as individual color text.
 * (i.e., text is drawn as a two-tone color graphic).
 */
void screen7(void);

/**
 * `MSX2` SCREEN 8 (GRAPHIC 7 mode).
 *
 * - 256 x 212 pixel, 8-bpp graphics mode.
 * - 256 colors/pixel of 256 colors (RGB332).
 * - Sprite mode 2.
 * - Pre-defined 16 colors for sprites.
 * - No color palette.
 *
 * **VRAM memory map**
 * | VRAM address | size   | contents                        |
 * |--------------|--------|---------------------------------|
 * | 0x00000      | 0xD400 | Pattern name table              |
 * | 0x0F000      | 0x0300 | Sprite pattern generator table  |
 * | 0x0F800      | 0x0200 | Sprite color table              |
 * | 0x0FA00      | 0x0080 | Sprite attribute table          |
 *
 * \note
 * In this mode, text is drawn as individual color text.
 * (i.e., text is drawn as a two-tone color graphic).
 */
void screen8(void);

/**
 * `MSX2+` SCREEN 10 (GRAPHIC 7 YJK/RGB mode).
 *
 * - 256 x 212 pixel, 8-bpp graphics mode.
 * - 12,499 colors (YJK), or 16 colors/pixel of 512 colors (RGB333).
 * - Sprite mode 2.
 *
 * **VRAM memory map**
 * | VRAM address | size   | contents                        |
 * |--------------|--------|---------------------------------|
 * | 0x00000      | 0xD400 | Pattern name table              |
 * | 0x0F000      | 0x0300 | Sprite pattern generator table  |
 * | 0x0F800      | 0x0200 | Sprite color table              |
 * | 0x0FA00      | 0x0080 | Sprite attribute table          |
 *
 * \note
 * In this mode, text is drawn as individual color text.
 * (i.e., text is drawn as a two-tone color graphic).
 *
 * \note
 * In this mode, text is drawn in the colors of the color palette as in SCREEN 5
 * and 7.
 */
void screen10(void);

/**
 * `MSX2+` SCREEN 12 (GRAPHIC 7 YJK mode).
 *
 * - 256 x 212 pixel, 8-bpp graphics mode.
 * - 19,268 colors (YJK).
 * - Sprite mode 2.
 *
 * **VRAM memory map**
 * | VRAM address | size   | contents                        |
 * |--------------|--------|---------------------------------|
 * | 0x00000      | 0xD400 | Pattern name table              |
 * | 0x0F000      | 0x0300 | Sprite pattern generator table  |
 * | 0x0F800      | 0x0200 | Sprite color table              |
 * | 0x0FA00      | 0x0080 | Sprite attribute table          |
 *
 * \note
 * In this mode, text is drawn as individual color text.
 * (i.e., text is drawn as a two-tone color graphic).
 *
 * \note
 * The pixel format for this mode is YJK/RGB, but it handles text foreground and
 * background colors in the same way as SCREEN 8. This may result in unexpected
 * colors in the text output feature.
 */
void screen12(void);

/** @} */

#endif // SCREEN_H_
