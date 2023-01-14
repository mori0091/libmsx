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
 * \file text.h
 * \brief Utility functions for text ouput.
 * \ingroup CUI
 */
/**
 * \defgroup CUI Text output
 * `#include <text.h>`
 *
 * The `text.h` provides some MSX-BASIC like utility functions for text output.
 * - locate(),
 * - color(),
 * - print(),
 * - and so on.
 *
 * \note
 * These functions (and also printf() standard C library functions) are
 * available after call to one of a SCREEN mode initialization function defined
 * in `screen.h`.
 *
 * \sa screen.h
 */

#pragma once

#ifndef TEXT_H_
#define TEXT_H_

#include "tty.h"
#include <stdint.h>

/**
 * \defgroup CUI_CONTROL Cursor position, Control characters.
 * \ingroup CUI
 * Cursor position, Control characters, Clear screen, etc.
 * @{
 */

/**
 * `MSX` Set cursor position.
 * \param x   x coordinate.
 * \param y   y coordinate.
 */
#define locate(x, y)                            \
  TTY_locate(x, y)

/**
 * `MSX` Line feed.
 */
#define newline()                               \
  TTY_linefeed()

/**
 * `MSX` Clear the screen and move the cursor to the top left corner.
 */
#define cls()                                   \
  TTY_cls()

/** @} */

/**
 * \defgroup CUI_COLOR Text color, Backdrop color
 * \ingroup CUI
 * Text color, Background color, Backdrop color.
 *
 * | SCREEN mode                   | Text foregrond    | Text background    | Backdrop         |
 * |-------------------------------|-------------------|--------------------|------------------|
 * | SCREEN 0 (TEXT 1)             | 16 color palette  | 16 color palette   | N/A              |
 * | SCREEN 0 (TEXT 2)             | 16 color palette  | 16 color palette   | N/A              |
 * | SCREEN 1 (GRAPHIC 1)          | 16 color palette  | 16 color palette   | 16 color palette |
 * | SCREEN 2 (GRAPHIC 2)          | 16 color palette  | 16 color palette   | 16 color palette |
 * | SCREEN 4 (GRAPHIC 3)          | 16 color palette  | 16 color palette   | 16 color palette |
 * | SCREEN 5 (GRAPHIC 4)          | 16 color palette  | 16 color palette   | 16 color palette |
 * | SCREEN 6 (GRAPHIC 5)          |  4 color palette  |  4 color palette   |  4 color palette |
 * | SCREEN 7 (GRAPHIC 6)          | 16 color palette  | 16 color palette   | 16 color palette |
 * | SCREEN 8 (GRAPHIC 7)          | RGB332            | RGB332             | RGB332           |
 * | SCREEN 10 (GRAPHIC 7 YJK/RGB) | 16 color palette  | 16 color palette   | 16 color palette |
 * | SCREEN 12 (GRAPHIC 7 YJK)     | A portion of YJK  | A portion of YJK   | 16 color palette |
 *
 * - 16 color palette (`0`..`15`)
 *   - `MSX`  Pre-defined 16 colors.
 *   - `MSX2` 16 colors out of 512 colors (RGB333).
 * - 4 color palette (`0`..`3`)
 *   - `MSX2` 4 colors out of 512 colors (RGB333).
 * - RGB332 (`0`..`255`)
 *   - `MSX2` 256 colors (RGB332).
 * - A portion of YJK (`0`..`255`)
 *   - `MSX2+` 19,268 colors (YJK)
 *   - A single YJK color value spans 4 pixels as follows:
 *     - 5-bits per pixel of Y component (`0`..`31`)
 *     - 6-bits per 4-pixels of J component (`-32`..`+31`)
 *     - 6-bits per 4-pixels of K component (`-32`..`+31`)
 *
 * \note
 * In YJK mode (SCREEN 12), text colors `0`-`255` are treated as in SCREEN 8.
 *
 * \note
 * However, a single YJK color value (especially the J and K components) spans 4
 * pixels. This can result in unexpected colors; in YJK mode, pixel-by-pixel
 * coloring is difficult to achieve.
 *
 * \note
 * In mixed YJK/RGB mode (SCREEN 10), unlike YJK mode (SCREEN 12), it is
 * possible to use YJK (12,499 colors) or a color palette (16 of 512 colors) per
 * pixel. Therefore, in mixed YJK/RGB mode, text are drawn with a color palette
 * of 16 colors.
 *
 * @{
 */

/**
 * `MSX` Set foreground color.
 * \param fg  foreground color of text.
 */
#define fg_color(fg)                            \
  TTY_set_fg_color(fg)

/**
 * `MSX` Set background color.
 * \param bg  background color of text.
 */
#define bg_color(bg)                            \
  TTY_set_bg_color(bg)

/**
 * `MSX` Set border color of the screen.
 * \param border  border color of the screen.
 */
#define border_color(border)                    \
  TTY_set_border_color(border)

/**
 * `MSX` Set foreground and background color.
 * \param fg  foreground color of text.
 * \param bg  background color of text.
 */
#define text_color(fg, bg)                      \
  TTY_set_text_color(fg, bg)

/**
 * `MSX` Set foreground, background, and border color.
 * \param fg      foreground color of text.
 * \param bg      background color of text.
 * \param border  border color of the screen.
 */
#define color(fg, bg, border)                   \
  TTY_set_color(fg, bg, border)

/** @} */

/**
 * \defgroup CUI_PRINT Text output
 * \ingroup CUI
 * Simple `print` functions.
 * @{
 */

/**
 * `MSX` Print string on the screen.
 * \param str  string
 */
void print(const char* str);

/**
 * `MSX` Print character on the screen.
 * \param c   character
 */
void printc(char c);

/**
 * `MSX` Print unsigned integer on the screen.
 * \param i   unsigned integer
 */
void printu(unsigned int i);

/**
 * `MSX` Print signed integer on the screen.
 * \param i   integer
 */
void printi(int i);

/**
 * `MSX` Print unsigned integer in hexadecimal on the screen.
 * \param x   unsigned integer
 */
void printx(uint16_t x);

/** @} */

#endif // TEXT_H_
