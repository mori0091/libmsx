// -*- coding: utf-8-unix -*-
/**
 * \file bios.h
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

#ifndef BIOS_H
#define BIOS_H

#include "bios_const.h"
#include "bios_entry.h"

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
 * BIOS : GTSTCK (00D5H / MAIN) `MSX`
 *
 * Get the status of the eight-way lever on the joystick or the arrow keys on
 * the keyboard.
 *
 * \param a  joystick number
 *           - `0` : keyboard (the arrow keys)
 *           - `1` : joystick #1
 *           - `2` : joystick #2
 *
 * \return   status of the eight-way lever or the arrow keys.
 *           - `0` : center
 *           - `1` : north
 *           - `2` : north east
 *           - `3` : east
 *           - `4` : south east
 *           - `5` : south
 *           - `6` : south west
 *           - `7` : west
 *           - `8` : north west
 *
 * \sa joypad_get_state()
 */
uint8_t msx_GTSTCK(const uint8_t a);

/**
 * BIOS : GTTRIG (00D8H / MAIN) `MSX`
 *
 * Get the status of the trigger button on the joystick or the SPACE key on the
 * keyboard.
 *
 * \param a  trigger button number
 *           - `0` : keyboard (the SPACE key)
 *           - `1` : trigger button #1 of the joystick #1
 *           - `2` : trigger button #1 of the joystick #2
 *           - `3` : trigger button #2 of the joystick #1
 *           - `4` : trigger button #2 of the joystick #2
 *
 * \return   status of the trigger button or the SPACE key.
 *           - `0x00` : released
 *           - `0xFF` : pressed
 *
 * \sa joypad_get_state()
 */
uint8_t msx_GTTRIG(const uint8_t a);

/**
 * BIOS : CHGCPU (0180H/MAIN) `MSXturboR`
 *
 * Change the CPU mode.
 *
 * \param mode  cpu mode, that shall be the following bit pattern:
 *           - `bit #7` : `LED`
 *             - `0` : Do not change the LED status
 *             - `1` : Turn the LED on and off according to the `CPU`
 *           - `bit #6..#2` : (unused, shall be all `0`)
 *           - `bit #1..#0` : `CPU`
 *             - `00` : Z80 mode
 *             - `01` : R800 ROM mode
 *             - `10` : R800 DRAM mode
 *             - `11` : (reserved)
 *
 * \note
 * This is a C language wrapper for the BIOS function `CHGCPU (0180H/MAIN)` for
 * MSXturboR.
 *
 * \attention
 * For MSX, MSX2, and MSX2+, the behavior is undefined.\n
 * Therefore, msx_set_cpu_mode() is recommended instead.
 *
 * \sa msx_set_cpu_mode()
 */
void msx_CHGCPU(const uint8_t mode);

/**
 * Change the CPU mode.
 *
 * \param mode  cpu mode, that shall be the following bit pattern:
 *           - `bit #7` : `LED`
 *             - `0` : Do not change the LED status
 *             - `1` : Turn the LED on and off according to the `CPU`
 *           - `bit #6..#2` : (unused, shall be all `0`)
 *           - `bit #1..#0` : `CPU`
 *             - `00` : Z80 mode
 *             - `01` : R800 ROM mode
 *             - `10` : R800 DRAM mode
 *             - `11` : (reserved)
 *
 * \note
 * This function is the C interface for the BIOS function`CHGCPU (0180H/MAIN)`
 * for MSXturboR.
 *
 * \note
 * For MSX, MSX2, and MSX2+, this function does nothing.\n
 * Therefore, unlike msx_CHGCPU(), this function can be used on any MSX platform.
 *
 * \sa msx_CHGCPU()
 */
void msx_set_cpu_mode(const uint8_t mode);

/**
 * BIOS : GETCPU (0183H/MAIN) `MSXturboR`
 *
 * Get the CPU mode.
 *
 * \return  cpu mode:
 *          - `0` : Z80 mode
 *          - `1` : R800 ROM mode
 *          - `2` : R800 DRAM mode
 *
 * \note
 * This is a C language wrapper for the BIOS function `GETCPU (0183H/MAIN)` for
 * MSXturboR.
 *
 * \attention
 * For MSX, MSX2, and MSX2+, the behavior is undefined.\n
 * Therefore, msx_get_cpu_mode() is recommended instead.
 *
 * \sa msx_get_cpu_mode()
 */
uint8_t msx_GETCPU(void);

/**
 * Get the CPU mode.
 *
 * \return  cpu mode:
 *          - `0` : Z80 mode
 *          - `1` : R800 ROM mode
 *          - `2` : R800 DRAM mode
 *
 * \note
 * This function is the C interface for the BIOS function`GETCPU (0183H/MAIN)`
 * for MSXturboR.
 *
 * \note
 * For MSX, MSX2, and MSX2+, this function always returns `0` (Z80 mode).\n
 * Therefore, unlike msx_GETCPU(), this function can be used on any MSX platform.
 *
 * \sa msx_GETCPU()
 */
uint8_t msx_get_cpu_mode(void);

#endif
