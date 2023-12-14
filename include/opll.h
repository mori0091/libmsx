// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2023 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file opll.h
 * \brief Device interface for MSX-MUSIC (OPLL).
 * \ingroup OPLL
 *
 * *Example*\n
 * The following code detects the MSX-MUSIC (OPLL) and plays a test tone if
 * detected.
 * \include opll_example/src/opll_example.c
 */
/**
 * \defgroup OPLL OPLL - YM2413 / MSX-MUSIC
 * `#include <opll.h>`
 */

#pragma once

#ifndef OPLL_H_
#define OPLL_H_

#include <stdint.h>

/**
 * \defgroup OPLL_DEVICE OPLL device interface
 * \ingroup OPLL
 * Inspect, discover, enable/disable to access registers, of the OPLL sound chip.
 *
 * *Example*\n
 * The following code detects the MSX-MUSIC (OPLL) and plays a test tone if
 * detected.
 * \include opll_example/src/opll_example.c
 *
 * @{
 */

/**
 * Device interface for OPLL sound chip.
 */
struct OPLL_Device {
  /**
   * `MSX` Function pointer to write data to a OPLL register.
   *
   * \param reg    OPLL register number
   *               - 0x00..0x07 : INSTURUMENT DATA registers
   *               - 0x0e       : RHYTHM control register
   *               - 0x0f       : TEST register
   *               - 0x10..0x18 : F-Number (LSB 8 bits) registers
   *               - 0x20..0x28 : SUS/KEY/BLOCK/F-Number (MSB) registers
   *               - 0x30..0x38 : INSTRUMENT/VOLUME registers
   * \param value  a value to be written.
   *
   * \post Interrupt is disabled.
   */
  void (* const write)(uint8_t reg, uint8_t value);
};

/**
 * OPLL Handle.
 *
 * \sa OPLL_find()
 * \sa OPLL_enable()
 * \sa OPLL_disable()
 */
struct OPLL {
  /**
   * Slot address of the OPLL.
   */
  uint8_t slot;
  /**
   * Version/Identifier of the OPLL.
   * - `1` if FMPAC,
   * - `2` or greater value if other OPLL,
   * - bit #7 is set if it is the internal OPLL.
   */
  uint8_t version;
  /**
   * Pointer to device interface.
   */
  const struct OPLL_Device * device;
};

/**
 * `MSX` Inspect whether OPLL is on the given slot.
 *
 * \param slot  a slot address
 *
 * \return Version/Identifier of the OPLL.
 *         - `0` if not OPLL,
 *         - `1` if FMPAC,
 *         - `2` or greater value if other OPLL,
 *         - bit #7 is set if it is the internal OPLL.
 */
uint8_t OPLL_inspect(uint8_t slot);

/**
 * `MSX` Find OPLL sound chip.
 *
 * \param opll [out]  pointer to a OPLL handle to be initialized.
 *
 * \return the slot address of OPLL if found, `0` otherwise.
 *
 * \post  If no OPLL found, `0` is set to `opll->slot`.
 * \post  If a OPLL found;
 *        - the slot address of OPLL is set to `opll->slot`,
 *        - `1` is set to `opll->version` if FMPAC,
 *        - `2` or greater value is set to `opll->version` if other OPLL,
 *        - bit #7 is set if it is the internal OPLL.
 *        - the device interface is set to `opll->device`.
 */
uint8_t OPLL_find(struct OPLL * opll);

/**
 * `MSX` Enable OPLL sound chip.
 *
 * Expose OPLL sound chip on the slot `opll->slot` and enable to access
 * registers of the sound chip.
 *
 * The `opll` shall point to a OPLL handle initialized by OPLL_find().
 *
 * \param opll  pointer to the OPLL handle.
 */
void OPLL_enable(const struct OPLL * opll);

/**
 * `MSX` Disable OPLL sound chip.
 *
 * Unexpose OPLL sound chip on the slot `opll->slot` and disable to access
 * registers of the sound chip.
 *
 * The `opll` shall point to a OPLL handle initialized by OPLL_find().
 *
 * \param opll  pointer to the OPLL handle.
 */
void OPLL_disable(const struct OPLL * opll);

/** @} */

#endif // OPLL_H_
