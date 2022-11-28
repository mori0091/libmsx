// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2022 Daishi Mori (mori0091)
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
 *
 * Functions corresponding to MSX-BASIC's following SCREEN modes:
 * - screen0()     : `SCREEN 0: WIDTH 40` (TEXT 1 mdoe),
 * - screen0_W80() : `SCREEN 0: WIDTH 80` (TEXT 2 mode) `MSX2` or later,
 * - screen1()     : `SCREEN 1: WIDTH 32` (GRAPHIC 1 mode),
 * - screen2()     : `SCREEN 2: WIDTH 32` (GRAPHIC 2 mode),
 * - screen2_PCG() : `SCREEN 2: WIDTH 32` (GRAPHIC 2 mode; PCG mode like as SCREEN 1).
 *
 * **All** above SCREEN modes supports:
 * - printf() and putchar() standard C library functions (`#include <stdio.h>`), and
 * - text utility functions of `libmsx` such as locate(), print() (`#include <text.h>`).
 *
 * \sa text.h
 */

#ifndef SCREEN_H_
#define SCREEN_H_

/**
 * `MSX` SCREEN 0: WIDTH 40 (TEXT 1 mode).
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
 * `MSX` SCREEN 1: WIDTH 32 (GRAPHIC 1 mode).
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
 * `MSX2` SCREEN 0: WIDTH 80 (TEXT 2 mode).
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

#endif // SCREEN_H_
