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
static const __at (0x002d) uint8_t MSX_SYSTEM_VERSION_CODE;

/**
 * Returns MSX system version code.
 *
 * \return 0 (MSX1), 1 (MSX2), 2 (MSX2+), 3 (MSXturboR), or else.
 */
inline uint8_t msx_get_version(void) {
  return MSX_SYSTEM_VERSION_CODE;
}

// Sub ROM

#endif
