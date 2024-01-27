// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2024 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file scc_buf.h
 * \brief Buffered access to SCC/SCC+ registers.
 * \ingroup SCC
 *
 * *Example*\n
 * The following code shows a sound driver/replayer template for the Konami
 * SCC/SCC+ sound chip.
 * \include scc_buf_example/src/scc_buf_example.c
*/

#pragma once

#ifndef SCC_BUF_H_
#define SCC_BUF_H_

#include <scc.h>

/**
 * \defgroup SCC_BUFFER SCC/SCC+ Buffer
 * \ingroup SCC
 * `#include <scc_buf.h>`
 * Buffered access to SCC/SCC+ registers.
 *
 * Provides simple APIs for sound driver developpers.
 *
 * *Example*\n
 * The following code shows a sound driver/replayer template for the Konami
 * SCC/SCC+ sound chip.
 * \include scc_buf_example/src/scc_buf_example.c
 *
 * @{
 */

struct SCC_buffer {
  /** Frequency division ratio for each of the five channels. */
  uint16_t fdr[5];
  /** Volume (0-15) for each of the five channels. */
  uint8_t  volume[5];
  /** Enable / Disable bit for each of the five channels. */
  uint8_t  channel_mask;
};

struct SCC_wave_buffer {
  /** Update-bit for each of five channels. */
  uint8_t updated;
  /** Waveform for each of five channels. */
  int8_t waveform[5][32];
};

/**
 * `MSX` Buffer for SCC/SCC+ waveform registers.
 *
 * \note Use SCC_set_waveform() for setting waveform buffers.
 */
extern struct SCC_wave_buffer scc_wave_buffer;

/**
 * `MSX` Buffer for SCC/SCC+ registers (except for waveform registers).
 *
 * \note Use SCC_set_waveform() for setting waveform buffers.
 */
extern struct SCC_buffer scc_buffer;

/**
 * `MSX` Initialize `scc_buffer` and internal waveform buffer.
 *
 * This must be called at least once before SCC_stop() or SCC_play().
 */
void SCC_init(void);

/**
 * `MSX` Set waveform to the internal waveform buffer, for the specified channel.
 *
 * \param ch        channel # 0..3 for SCC, 0..4 for SCC+.
 * \param waveform  pointer to 32 bytes waveform pattern.
 *
 * \note
 * The waveform is applied onto SCC/SCC+ when SCC_play() is called.
 *
 * \note
 * In case of SCC and SCC compatible mode of SCC+, waveform register of the 4th
 * channel (ch = 3) and the 5th channel (ch = 4) are shared.
 */
void SCC_set_waveform(uint8_t ch, const int8_t waveform[32]);

/**
 * `MSX` Stop (Pause) playing sound on SCC/SCC+.
 *
 * This function sets all volume registers to `0`.
 *
 * \param scc  pointer to the SCC handle.
 * \post Interrupt is disabled.
 *
 * \sa SCC_find()
 * \sa SCC_set_mode()
 * \sa SCC_get_mode()
 *
 * \note
 * This function simply sets all channels' volume level to 0.
 */
void SCC_stop(struct SCC * scc);

/**
 * `MSX` Play sound on SCC/SCC+.
 *
 * This function applies the value of `scc_buffer` to SCC/SCC+. It also applies
 * the value of the internal waveform buffer if it has been updated.
 *
 * \param scc  pointer to the SCC handle.
 * \post Interrupt is disabled.
 *
 * \sa SCC_find()
 * \sa SCC_set_mode()
 * \sa SCC_get_mode()
 */
void SCC_play(struct SCC * scc);

/** @} */

#endif // SCC_BUF_H_
