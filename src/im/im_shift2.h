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

#ifndef IM_SHIFT2_H_
#define IM_SHIFT2_H_

#include <im.h>

void im_shift2_l_BM8x8(uint8_t sft, const BM8x8 * src, const BM8x8 * oth, BM8x8 * dst);
void im_shift2_l_v_BM8x8(uint8_t sft, const BM8x8 * src, const BM8x8 * oth);
void im_shift2_r_BM8x8(uint8_t sft, const BM8x8 * src, const BM8x8 * oth, BM8x8 * dst);
void im_shift2_r_v_BM8x8(uint8_t sft, const BM8x8 * src, const BM8x8 * oth);

#endif // IM_SHIFT2_H_
