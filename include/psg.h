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
 * \file psg.h
 * \brief Device interface for PSG (AY-3-8910) an internal sound chip.
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
 * `MSX` Initialize PSG registers R#0 to R#13 with `psg_reg_initial_vector`.
 */
void psg_init(void);

/**
 * `MSX` Read the PSG register value.
 *
 * \param reg    PSG register number.
 */
inline uint8_t psg_get(uint8_t reg) {
  psg_port0 = reg;
  return psg_port2;
}

/**
 * `MSX` Write the PSG register value.
 *
 * \param reg    PSG register number.
 * \param val    the value.
 */
inline void psg_set(uint8_t reg, uint8_t val) {
  psg_port0 = reg;
  psg_port1 = val;
}

#endif
