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
 * \file text.h
 * \brief Utility functions for text ouput.
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

#ifndef TEXT_H_
#define TEXT_H_

#include "tty.h"
#include <stdint.h>

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

#endif // TEXT_H_
