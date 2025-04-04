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
 * \file bios.h
 * \brief C language I/F for MSX BIOS routines.
 * \ingroup BIOS
 */
/**
 * \defgroup BIOS BIOS - Basic Input/Output System
 * \ingroup LIBMSX_SYS
 * `#include <bios.h>`
 */

#pragma once

#ifndef BIOS_H
#define BIOS_H

#include <stdint.h>
#include "bios_const.h"
#include "bios_entry.h"

#ifndef __SDCCCALL
#define __sdcccall(x)
#endif

/**
 * \defgroup BIOS_CONFIG System configuration
 * \ingroup BIOS
 * Get the system version, VSYNC frequency, etc.
 * @{
 */

/**
 * `MSX` Size of a page (16KiB) in bytes.
 */
#define PAGE_SIZE           (uint16_t)(0x4000)

/**
 * `MSX` Constructs base address of the specified page.
 *
 * \param p  page number (`0`..`3`)
 * \return   base address of the page #`p`.
 */
#define PAGE_ADDR(p)        ((void *)(PAGE_SIZE * (p)))

/**
 * `MSX` Initial slot addresses for each page.
 */
extern const uint8_t libmsx_initial_slots[4];

/**
 * `MSX` The cartrige slot address.
 */
#define CARTRIDGE_SLOT  (libmsx_initial_slots[1])

/**
 * `MSX` Returns MSX system version code.
 *
 * \return 0 (MSX1), 1 (MSX2), 2 (MSX2+), 3 (MSXturboR), or else.
 */
inline uint8_t msx_get_version(void) {
  return MSX_SYSTEM_VERSION_CODE;
}

/**
 * `MSX` Returns VSYNC frequency in Hz.
 *
 * \return 50 (PAL/SECAM), or 60 (NTSC)
 */
inline uint8_t msx_get_vsync_frequency(void) {
  return ((INTERNATIONAL_ID_1 & 0x80) ? 50 : 60);
}

/** @} */

/**
 * \defgroup BIOS_SLOT Inter-slot Read / Write / Call
 * \ingroup BIOS
 * Inter-slot Read / Write / Call interface.
 * @{
 */

/**
 * `MSX` BIOS : RDSLT (000CH / MAIN).
 *
 * Read value from the given address of the given slot.
 *
 * \param slot  the slot
 * \param addr  the address to read
 *
 * \return value of the address of the slot.
 *
 * \post Interrupt is disabled.
 */
uint8_t msx_RDSLT(uint8_t slot, void * addr);

/**
 * `MSX` BIOS : WRSLT (0014H / MAIN).
 *
 * Write a value to the given address of the given slot.
 *
 * \param slot  the slot
 * \param addr  the address to write
 * \param value a value to be written
 *
 * \post Interrupt is disabled.
 */
void msx_WRSLT(uint8_t slot, void * addr, uint8_t value) __sdcccall(0);

/**
 * `MSX` BIOS : ENASLT (0024H / MAIN).
 *
 * Switch the page including the given address to the given slot's corresponding page.
 *
 * \param slot  the slot
 * \param addr  an address in the page
 *
 * \post Interrupt is disabled.
 */
void msx_ENASLT(uint8_t slot, void * addr);

/**
 * `MSX` Get the current slot of a 16KiB page that including the given address.
 *
 * \param addr  the address
 *
 * \return the current slot of that page.
 */
uint8_t msx_get_slot(void * addr);

/**
 * `MSX` BIOS : RSLREG (0138H / MAIN).
 *
 * Read value from the primary slot select register.
 *
 * \return value read from the primary slot select register.
 */
uint8_t msx_RSLREG(void);

/**
 * `MSX` BIOS : WSLREG (013BH / MAIN).
 *
 * Write a value to the primary slot select register.
 *
 * \param value a value to be written to the primary slot select register.
 */
void msx_WSLREG(uint8_t value);

/** @} */

/**
 * \defgroup BIOS_JOYPAD Joystick / Joypad
 * \ingroup BIOS
 * Joystick / Joypad interface.
 * @{
 */

/**
 * `MSX` BIOS : GTSTCK (00D5H / MAIN)
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
 * `MSX` BIOS : GTTRIG (00D8H / MAIN)
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

/** @} */

/**
 * \defgroup BIOS_CPU Z80 / R800 CPU mode
 * \ingroup BIOS
 * Change / Get CPU mode.
 * @{
 */

/**
 * `MSXturboR` BIOS : CHGCPU (0180H/MAIN)
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
 * `MSX` Change the CPU mode.
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
 * `MSXturboR` BIOS : GETCPU (0183H/MAIN)
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
 * `MSX` Get the CPU mode.
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

/** @} */

#ifndef __SDCCCALL
#undef __sdcccall
#endif

#endif
