// -*- coding: utf-8-unix -*-
/**
 * \file bios.h
 *
 * Copyright (c) 2021 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

#ifndef BIOS_H
#define BIOS_H

#pragma once

#include <stdint.h>

/**
 * Major version code of MSX BIOS.
 */
static const __at (0x002d) uint8_t MSX_SYSTEM_VERSION_CODE;

/**
 * International identifier code #1 of MSX BIOS.
 */
static const __at (0x002b) uint8_t INTERNATIONAL_ID_1;

/**
 * International identifier code #2 of MSX BIOS.
 */
static const __at (0x002c) uint8_t INTERNATIONAL_ID_2;

/**
 * Returns MSX system version code.
 *
 * \return 0 (MSX1), 1 (MSX2), 2 (MSX2+), 3 (MSXturboR), or else.
 */
inline uint8_t msx_get_version(void) {
  return MSX_SYSTEM_VERSION_CODE;
}

/**
 * Returns VSYNC frequency in Hz.
 *
 * \return 50 (PAL/SECAM), or 60 (NTSC)
 */
inline uint8_t msx_get_vsync_frequency(void) {
  return ((INTERNATIONAL_ID_1 & 0x80) ? 50 : 60);
}

/**
 * Change the CPU mode based on the given `mode`.
 *
 * \note
 * - This function is the C interface to the `CHGCPU (0180H/MAIN)` BIOS function
 *   for MSXturboR.
 * - In case of MSX, MSX2, or MSX2+, this function does nothing.
 *
 *
 * The parameter `mode` shall be the following bit pattern:
 * - bit #7     : LED
 * - bit #6..#2 : (unused, shall be all '0')
 * - bit #1..#0 : CPU
 *
 * **LED**
 * - `0` : Do not change the LED status
 * - `1` : Turn the LED on and off according to the **CPU**
 *
 * **CPU**
 * - `00` : Z80 mode
 * - `01` : R800 ROM mode
 * - `10` : R800 DRAM mode
 * - `11` : (reserved)
 *
 * \param mode    cpu mode
 */
void msx_set_cpu_mode(const uint8_t mode);

#endif
