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
 * \file psg.h
 * \brief Device interface for PSG (AY-3-8910) an internal sound chip.
 * \ingroup PSG
 */
/**
 * \defgroup PSG PSG - AY-3-8910 / Programmable Sound Generator
 * \ingroup LIBMSX_SOUNDCHIP
 * `#include <psg.h>`, `#include <ay_3_8910.h>`
 */

#pragma once

#ifndef PSG_H
#define PSG_H

#include <stdint.h>

#include "io.h"

/**
 * \defgroup PSG_REGISTER PSG registers
 * \ingroup PSG
 * Read / Write PSG registers
 * @{
 */

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
uint8_t psg_get(uint8_t reg);

/**
 * `MSX` Write the PSG register value.
 *
 * \param reg    PSG register number.
 * \param val    the value.
 */
void psg_set(uint8_t reg, uint8_t val);

/** @} */

#endif
