// -*- coding: utf-8-unix -*-
/**
 * \file psg.h
 *
 * Copyright (c) 2021 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */

#pragma once

#ifndef PSG_H
#define PSG_H

#include <stdint.h>

#include "io.h"

/**
 * Initial values for PSG registers R#0 to R#13.
 */
extern const uint8_t psg_reg_initial_vector[14];

/**
 * Initialize PSG registers R#0 to R#13 with `psg_reg_initial_vector`.
 */
void psg_init(void);

/**
 * Read the PSG register value.
 *
 * \param reg    PSG register number.
 */
inline uint8_t psg_get(uint8_t reg) {
  psg_port0 = reg;
  return psg_port2;
}

/**
 * Write the PSG register value.
 *
 * \param reg    PSG register number.
 * \param val    the value.
 */
inline void psg_set(uint8_t reg, uint8_t val) {
  psg_port0 = reg;
  psg_port1 = val;
}

#endif
