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
 * \file tile.h
 */

#ifndef TILE_H_
#define TILE_H_

#include <msx.h>
#include <screen.h>
#include <stdint.h>
#include <text.h>
#include <im.h>

#include <string.h>

#define ARRAY_LENGTH(a) (sizeof(a)/sizeof((a)[0]))

/* Configurations for GRAPHIC 2 mode (SCREEN 2) */
#define SCREEN_MODE     VDP_SCREEN_MODE_GRAPHIC_2
#define PATTERNS        (0x00000) // pattern generator table
#define IMAGE           (0x01800) // pattern name table
#define COLORS          (0x02000) // color table
#define SPRITES         (0x01B00) // sprite attribute table
#define SPRITE_PATTERNS (0x03800) // sprite pattern generator table
#define SIZE_OF_PATTERNS (0x1800) // size of pattern generator table
#define SIZE_OF_IMAGE    (0x0300) // size of pattern name table
#define SIZE_OF_COLORS   (0x1800) // size of color table

#define COLUMNS (32)
#define LINES   (24)

extern BM8x8 tileset[256];

extern const BM8x8 masks[6];

extern const SP16x16 sprite_masks[5];

// Read into the tileset from the 256-character pattern in the main ROM.
void copy_tileset_from_MainROM(void);

// Write the tileset to the pattern generator table in VRAM.
void copy_tileset_to_VRAM(void);

void demo_8x8_unary_operators(void);
void demo_8x8_binary_operators(void);
void demo_8x8_shift_operators(void);

void demo_16x16_unary_operators(void);
void demo_16x16_binary_operators(void);
void demo_16x16_shift_operators(void);

void demo_8x8_vec_shift_operators(void);
void demo_8x8_tilemap_operator(void);
void demo_16x16_trimming_operator(void);

typedef void (*BM8x8_UnaryOp)(const BM8x8 * src, BM8x8 * dst);
typedef void (*BM8x8_BinaryOp)(const BM8x8 * src1, const BM8x8 * src2, BM8x8 * dst);
typedef void (*BM8x8_ScalarOp)(uint8_t a, const BM8x8 * src, BM8x8 * dst);

void apply_unary_op_BM8x8(BM8x8_UnaryOp f);
void apply_binary_op_BM8x8(BM8x8_BinaryOp f, const BM8x8 * rhs);
void apply_scalar_op_BM8x8(uint8_t lhs, BM8x8_ScalarOp f);

typedef void (*BM8x8_UnaryOp_v)(const BM8x8 * src);
typedef void (*BM8x8_BinaryOp_v)(const BM8x8 * src1, const BM8x8 * src2);
typedef void (*BM8x8_ScalarOp_v)(uint8_t a, const BM8x8 * src);

void apply_unary_op_v_BM8x8(BM8x8_UnaryOp_v f);
void apply_binary_op_v_BM8x8(BM8x8_BinaryOp_v f, const BM8x8 * rhs);
void apply_scalar_op_v_BM8x8(uint8_t lhs, BM8x8_ScalarOp_v f);

typedef void (*SP16x16_UnaryOp)(const SP16x16 * src, SP16x16 * dst);
typedef void (*SP16x16_BinaryOp)(const SP16x16 * src1, const SP16x16 * src2, SP16x16 * dst);
typedef void (*SP16x16_ScalarOp)(uint8_t a, const SP16x16 * src, SP16x16 * dst);

void apply_unary_op_SP16x16(SP16x16_UnaryOp f);
void apply_binary_op_SP16x16(SP16x16_BinaryOp f, const SP16x16 * rhs);
void apply_scalar_op_SP16x16(uint8_t lhs, SP16x16_ScalarOp f);

typedef void (*SP16x16_UnaryOp_v)(const SP16x16 * src);
typedef void (*SP16x16_ScalarOp_v)(uint8_t a, const SP16x16 * src);
typedef void (*SP16x16_BinaryOp_v)(const SP16x16 * src1, const SP16x16 * src2);

void apply_unary_op_v_SP16x16(SP16x16_UnaryOp_v f);
void apply_binary_op_v_SP16x16(SP16x16_BinaryOp_v f, const SP16x16 * rhs);
void apply_scalar_op_v_SP16x16(uint8_t lhs, SP16x16_ScalarOp_v f);

#endif // TILE_H_
