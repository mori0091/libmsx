// -*- coding: utf-8-unix -*-
/**
 * \file vdp_internal.h
 *
 * Copyright (c) 2021-2023 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

#ifndef VDP_INTERNAL_H
#define VDP_INTERNAL_H

#pragma once

#include <stdint.h>

extern volatile enum vdp_screen_mode screen_mode;
extern volatile uint8_t sprite_mode;

#if (__SDCCCALL == 1)

extern void (* vmem__fnptr_write_chunk)(const uint8_t * p, uint16_t n);
extern void (* vmem__fnptr_read_chunk)(uint8_t * p, uint16_t n);

void vmem_read_chunk_1(uint8_t * p, uint16_t n);
void vmem_read_chunk_2(uint8_t * p, uint16_t n);
void vmem_write_chunk_1(const uint8_t * p, uint16_t n);
void vmem_write_chunk_2(const uint8_t * p, uint16_t n);

#endif

#endif
