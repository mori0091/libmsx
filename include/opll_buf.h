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
 * \file opll_buf.h
 * \brief Buffered access to OPLL registers.
 * \ingroup OPLL
 *
 * *Example*\n
 * The following code shows a sound driver/replayer template for the MSX-MUSIC
 * (OPLL).
 * \include opll_buf_example/src/opll_buf_example.c
 */

#pragma once

#ifndef OPLL_BUF_H_
#define OPLL_BUF_H_

#include <opll.h>

#include <stdbool.h>

/**
 * \defgroup OPLL_BUFFER OPLL Buffer
 * \ingroup OPLL
 * `#include <opll_buf.h>`
 * Buffered access to OPLL registers.
 *
 * Provides simple APIs for sound driver developpers.
 *
 * *Example*\n
 * The following code shows a sound driver/replayer template for the MSX-MUSIC
 * (OPLL).
 * \include opll_buf_example/src/opll_buf_example.c
 *
 * @{
 */

/**
 * `MSX` Cache of OPLL registers.
 *
 * \sa OPLL_put()
 */
extern uint8_t opll_buffer[64];

/**
 * `MSX` Initialize internal buffer for OPLL.
 *
 * This must be called at least once before OPLL_stop() or OPLL_play().
 */
void OPLL_init(void);

/**
 * `MSX` Put a pair of OPLL register number and its value to the internal buffer.
 *
 * Put the pair of OPLL register #`reg` and its value `val` into the internal
 * FIFO buffer. Then, by calling `OPLL_play()`, all values in the FIFO buffer
 * are written to OPLL.
 *
 * \param reg    OPLL register number
 *               - 0x00..0x07 : INSTURUMENT DATA registers
 *               - 0x0e       : RHYTHM control register
 *               - 0x0f       : TEST register
 *               - 0x10..0x18 : F-Number (LSB 8 bits) registers
 *               - 0x20..0x28 : SUS/KEY/BLOCK/F-Number (MSB) registers
 *               - 0x30..0x38 : INSTRUMENT/VOLUME registers
 * \param val    a value to be written to the register.
 */
void OPLL_put(uint8_t reg, uint8_t val);

/**
 * `MSX` Turn RHYTHM mode on/off.
 *
 * \param on   Specify the mode:
 *             - `true`  : 6 channels + RHYTHM mode.
 *             - `false` : 9 channels mode.
 */
void OPLL_rhythm_mode(bool on);

/**
 * `MSX` Key-on/off RHYTHM set.
 *
 * \param val  Key-on/off switch for each RHYTHM set:
 *             - bit #0 : HH (high-hat)
 *             - bit #1 : TOP-CY (cymbal)
 *             - bit #2 : TOM (tom-tom)
 *             - bit #3 : SD (snare drum)
 *             - bit #4 : BD (bass drum)
 *
 * \pre The mode shall be 6 channels + RHYTHM mode.
 *
 * \sa OPLL_rhythm_mode()
 */
void OPLL_rhythm(uint8_t val);

/**
 * `MSX` Stop (Pause) playing sound on OPLL
 *
 * \param opll  pointer to the OPLL handle.
 * \post Interrupt is disabled.
 *
 * \sa OPLL_find()
 *
 * \note
 * This function simply sets all channels to KEY-OFF.
 */
void OPLL_stop(struct OPLL * opll);

/**
 * `MSX` Play sound on OPLL
 *
 * This function applies the values in the internal buffer to OPLL.
 *
 * \param opll  pointer to the OPLL handle.
 * \post Interrupt is disabled.
 *
 * \sa OPLL_find()
 */
void OPLL_play(struct OPLL * opll);

/** @} */

#endif // OPLL_BUF_H_
