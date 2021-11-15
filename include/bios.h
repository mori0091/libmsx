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

// Main ROM
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

// Sub ROM

#endif
