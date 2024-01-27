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
 * \file audio_cb.h
 * \brief Callback functions of the libmsx audio replayer.
 * \ingroup AUDIO
 */

#ifndef AUDIO_CB_H_
#define AUDIO_CB_H_

#include <audio_dec.h>

#include <stdint.h>

/**
 * \defgroup AUDIO_CALLBACK Callback functions of the libmsx audio replayer.
 * \ingroup AUDIO
 * `#include <audio_cb.h>`
 *
 * Callback functions of the libmsx audio replayer.
 *
 * Either music loader or decoder implementation shall use these functions.
 *
 * \sa AUDIO_DECODER
 * @{
 */

/**
 * `MSX` Callback function called by the music loader or decoder when setting or
 * changing the BGM.
 *
 * Notify the replayer that BGM is changed or set to the decoder, and its
 * default frequency.
 *
 * Implementations of either the music loader or decoder should call this
 * function when the background music is changed or set to the decoder.
 *
 * \param song_rate  Default frequency of the song.
 *                   (may be 0 if unknown)
 * \param decoder    Pointer to an AudioDecoder object.
 *
 * \post interrupts is disabled.
 */
void audio_cb_bgm_changed(uint8_t song_rate, const AudioDecoder * decoder);

/**
 * `MSX` Callback function called by the music loader or decoder when setting or
 * changing the SFX.
 *
 * Notify the replayer that SFX is changed or set to the decoder, and its
 * default frequency.
 *
 * Implementations of either the music loader or decoder should call this
 * function when the sound effects is changed or set to the decoder.
 *
 * \param song_rate  Default frequency of the song.
 *                   (may be 0 if unknown)
 * \param decoder    Pointer to an AudioDecoder object.
 *
 * \post interrupts is disabled.
 */
void audio_cb_sfx_changed(uint8_t song_rate, const AudioDecoder * decoder);

/**
 * `MSX` Set the replayer to use SCC compatible mode or SCC+ mode.
 *
 * \param mode `1` for SCC mode, `2` for SCC+ mode, others are ignored.
 *
 * \post interrupts is enabled.
 */
void audio_cb_scc_mode(uint8_t mode);

/** @} */

#endif // AUDIO_CB_H_
