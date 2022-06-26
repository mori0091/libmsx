// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file screen1.h
 */

#ifndef SCREEN1_H_
#define SCREEN1_H_

#include <stdint.h>

/* Configurations for GRAPHIC 1 mode (SCREEN 1) */
#define SCREEN_MODE     VDP_SCREEN_MODE_GRAPHIC_1
#define PATTERNS        (0x00000) // pattern generator table
#define IMAGE           (0x01800) // pattern name table
#define COLORS          (0x02000) // color table
#define SPRITES         (0x01B00) // sprite attribute table
#define SPRITE_PATTERNS (0x03800) // sprite pattern generator table
#define SIZE_OF_PATTERNS (0x0800) // size of pattern generator table
#define SIZE_OF_IMAGE    (0x0300) // size of pattern name table
#define SIZE_OF_COLORS   (32)     // size of color table

void screen1(void);
void init_vdp(void);
void clear_vmem(void);

void locate(uint8_t x, uint8_t y);
void putc(int c);
void puts(const char* str);
void puti(int i);

#endif // SCREEN1_H_
