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
 * \file tty.h
 * \brief Teletype/console device interface.
 */

#pragma once

#ifndef TTY_H_
#define TTY_H_

#include <stdint.h>

/**
 * Teletype/console device interface for hardware / screen mode abstraction.
 */
struct TTY_Device {
  /** Columns of the text screen. */
  uint8_t columns;
  /** Linex of the text screen. */
  uint8_t lines;
  /** Clear the text screen. */
  void (* clear_screen)(void);
  /** Render a character at the cursor position. */
  void (* render_char)(uint8_t c);
  /** Set foreground and background color of text. */
  void (* set_text_color)(uint8_t fg, uint8_t bg);
  /** Set border color of the screen. */
  void (* set_border_color)(uint8_t border);
};

extern const struct TTY_Device * TTY_device;

/**
 * `MSX` Set cursor position.
 * \param x   x coordinate.
 * \param y   y coordinate.
 */
void TTY_locate(uint8_t x, uint8_t y);

/**
 * `MSX` Line feed.
 */
void TTY_linefeed(void);

/**
 * `MSX` Clear the screen and move the cursor to the top left corner.
 */
void TTY_cls(void);

/**
 * `MSX` Send a byte to the TTY device.
 * \param c   a byte.
 */
void TTY_put(uint8_t c);

/**
 * `MSX` Set foreground color.
 * \param fg  foreground color of text.
 */
void TTY_set_fg_color(uint8_t fg);

/**
 * `MSX` Set background color.
 * \param bg  background color of text.
 */
void TTY_set_bg_color(uint8_t bg);

/**
 * `MSX` Set border color of the screen.
 * \param border  border color of the screen.
 */
void TTY_set_border_color(uint8_t border);

/**
 * `MSX` Set foreground and background color.
 * \param fg  foreground color of text.
 * \param bg  background color of text.
 */
void TTY_set_text_color(uint8_t fg, uint8_t bg);

/**
 * `MSX` Set foreground, background, and border color.
 * \param fg      foreground color of text.
 * \param bg      background color of text.
 * \param border  border color of the screen.
 */
void TTY_set_color(uint8_t fg, uint8_t bg, uint8_t border);

#endif // TTY_H_
