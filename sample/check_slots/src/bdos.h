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
 * \file bdos.h
 */

#pragma once

#ifndef BDOS_H_
#define BDOS_H_

#include <stdint.h>

struct z80_reg {
  uint16_t af;
  uint16_t bc;
  uint16_t de;
  uint16_t hl;
};

#ifndef __SDCCCALL
#define __sdcccall(x)
#endif

/**
 * Wrapper for BDOS call.
 * \param reg [inout]  pointer to value of CPU registers.
 */
void bdos(uint8_t c, struct z80_reg * reg) __sdcccall(0);

#ifndef __SDCCCALL
#undef __sdcccall
#endif

#endif // BDOS_H_
