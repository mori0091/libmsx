// -*- coding: utf-8-unix -*-
/**
 * \file bios_entry.h
 * \brief Assembler entry points for MSX BIOS functions.
 * \note This defines address and name of entry points.
 *
 * Copyright (c) 2022 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */

#pragma once

#ifndef BIOS_ENTRY_H
#define BIOS_ENTRY_H

#include <stdint.h>

#include "config.h"

/**
 * BIOS : RDSLT (000CH / MAIN) `MSX`.
 *
 * Read value from the given address of the given slot.
 *
 * \param A  slot
 * \param HL address to read
 *
 * \retval A value of the address of the slot.
 *
 * \post AF, BC, DE registers will be changed.
 * \post Interrupt is disabled.
 *
 * \sa msx_RDSLT()
 */
static __at(0x000c) const uint8_t RDSLT;

/**
 * BIOS : WRSLT (0014H / MAIN) `MSX`.
 *
 * Write a value to the given address of the given slot.
 *
 * \param A  slot
 * \param HL address to write
 * \param E  a value to be written
 *
 * \post AF, BC, D registers are changed.
 * \post Interrupt is disabled.
 *
 * \sa msx_WRSLT()
 */
static __at(0x0014) const uint8_t WRSLT;

/**
 * BIOS : CALSLT (001CH / MAIN) `MSX`.
 *
 * Inter slot call to the given address of the given slot.
 *
 * \param IY slot (higher byte of `IY` register)
 * \param IX address to be called
 *
 * \post CPU registers are changed according to the results of the routines called.
 */
static __at(0x001c) const uint8_t CALSLT;

/**
 * BIOS : ENASLT (0024H / MAIN) `MSX`.
 *
 * Switch the page including the given address to the given slot's corresponding page.
 *
 * \param A  slot
 * \param HL address
 *
 * \post All registers are changed.
 * \post Interrupt is disabled.
 */
static __at(0x0024) const uint8_t ENASLT;

/**
 * BIOS : CALLF (0030H / MAIN) `MSX`.
 *
 * Inter slot call to the given address of the given slot.
 *
 * Parameters shall be passed as inline parameter forms as follows:
 * ~~~ asm
 * rst 30h    ; same as "call _CALLF"
 * db  00h    ; slot #0
 * dw  006ch  ; address 006ch
 * ~~~
 *
 * \post CPU registers are changed according to the results of the routines called.
 */
static __at(0x0030) const uint8_t CALLF;

/**
 * BIOS : RSLREG (0138H / MAIN) `MSX`.
 *
 * Read value from the primary slot select register.
 *
 * \retval A value
 */
static __at(0x0138) const uint8_t RSLREG;

/**
 * BIOS : WSLREG (013BH / MAIN) `MSX`.
 *
 * Write a value to the primary slot select register.
 *
 * \param A value
 */
static __at(0x013b) const uint8_t WSLREG;

/**
 * BIOS : GTSTCK (00D5H / MAIN) `MSX`.
 *
 * Get the status of the eight-way lever on the joystick or the arrow keys on
 * the keyboard.
 *
 * \param A joystick number
 *           - `0` : keyboard (the arrow keys)
 *           - `1` : joystick #1
 *           - `2` : joystick #2
 *
 * \retval A status of the eight-way lever or the arrow keys.
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
 * \post ALL CPU registers will be changed.
 *
 * \sa msx_GTSTCK()
 * \sa joypad_get_state()
 * \sa MSX Datapack Volume 2, pp.70 (w/ error)
 * \sa MSX Datapack Volume 2, pp.612
 */
static __at(0x00D5) const uint8_t GTSTCK;

/**
 * BIOS : GTTRIG (00D8H / MAIN) `MSX`
 *
 * Get the status of the trigger button on the joystick or the SPACE key on the
 * keyboard.
 *
 * \param A  trigger button number
 *           - `0` : keyboard (the SPACE key)
 *           - `1` : trigger button #1 of the joystick #1
 *           - `2` : trigger button #1 of the joystick #2
 *           - `3` : trigger button #2 of the joystick #1
 *           - `4` : trigger button #2 of the joystick #2
 *
 * \retval A status of the trigger button or the SPACE key.
 *           - `0x00` : released
 *           - `0xFF` : pressed
 *
 * \sa msx_GTTRIG()
 * \sa joypad_get_state()
 * \sa MSX Datapack Volume 2, pp.71
 * \sa MSX Datapack Volume 2, pp.613 (w/ error)
 */
static __at(0x00D8) const uint8_t GTTRIG;

/**
 * BIOS : CHGCPU (0180H/MAIN) `MSXturboR`
 *
 * Change the CPU mode.
 *
 * \param A  cpu mode, that shall be the following bit pattern:
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
 * \post Interrupts will be enabled.
 *
 * \note
 * The values of all registers except the `R` register will be inherited by the
 * changed CPU.
 *
 * \note
 * R800 DRAM mode is the default and fastest CPU mode for MSXturboR. DRAM access
 * is faster than ROM access on R800 CPUs. Therefore, the MSXturboR boot code
 * copies the system software (i.e. BIOS ROM code) to DRAM during the boot
 * sequence, and maps that DRAM area to the address space instead of BIOS ROM.
 *
 * \attention
 * Changing the CPU mode is done by simply changing the settings of the system
 * control LSI (S1990). Therefore, if you change the CPU mode to Z80 mode or
 * R800 ROM mode, then change the contents of the DRAM area where was used for
 * system software, and then change the CPU mode to R800 DRAM mode, the system
 * will crash. System software is transferred to the DRAM only at system
 * startup.
 *
 * \sa msx_CHGCPU()
 * \sa msx_set_cpu_mode()
 * \sa MSX Datapack Volume 3, pp.13
 * \sa MSX Datapack Volume 3, pp.25
 */
static __at(0x0180) const uint8_t CHGCPU;

/**
 * BIOS : GETCPU (0183H/MAIN) `MSXturboR`
 *
 * Get the CPU mode.
 *
 * \retval A  cpu mode:
 *         - `0` : Z80 mode
 *         - `1` : R800 ROM mode
 *         - `2` : R800 DRAM mode
 *
 * \post `F` register will be changed.
 *
 * \sa msx_GETCPU()
 * \sa msx_get_cpu_mode()
 * \sa MSX Datapack Volume 3, pp.13
 * \sa MSX Datapack Volume 3, pp.27
 */
static __at(0x0183) const uint8_t GETCPU;

#endif
