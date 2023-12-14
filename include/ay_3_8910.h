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
 * \file ay_3_8910.h
 * \brief Buffer for PSG (AY-3-8910) registers.
 * \ingroup PSG
 *
 * *Example*\n
 * The following code shows a sound driver/replayer template for the PSG.
 * \include psg_buf_example/src/psg_buf_example.c
 */

#pragma once

#ifndef AY_3_8910_H
#define AY_3_8910_H

#include "psg.h"

/**
 * \defgroup PSG_BUFFER PSG Buffer
 * \ingroup PSG
 * Buffered access to PSG registers.
 *
 * *Example*\n
 * The following code shows a sound driver/replayer template for the PSG.
 * \include psg_buf_example/src/psg_buf_example.c
 *
 * @{
 */

/**
 * Buffer for PSG (AY-3-8910) registers.
 */
extern uint8_t ay_3_8910_buffer[14];

/**
 * `MSX` Initialize ay_3_8910_buffer[].
 */
void ay_3_8910_init(void);

/**
 * `MSX` Stop (Pause) playing sound on PSG.
 *
 * This function sets all volume registers to `0`.
 *
 * \note
 * This function simply sets all channels' volume level to 0.
 */
void ay_3_8910_stop(void);

/**
 * `MSX` Write the buffer contents to PSG registers.
 *
 * \note
 * PSG R#13 register is overwritten only if ay_3_8910_buffer[13] had been
 * updated, and then the ay_3_8910_buffer[13] is reinitialized.
 *
 * \note
 * Any other PSG registers (R#0..12) are always overwritten with corresponding
 * buffer value.
 */
void ay_3_8910_play(void);

/** @} */

#endif
