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
 * \file audio.h
 * \brief The libmsx audio replayer.
 * \ingroup AUDIO
 */
/**
 * \defgroup AUDIO The libmsx audio replayer.
 * `#include <audio.h>`
 */

#ifndef AUDIO_H_
#define AUDIO_H_

#include <stdbool.h>
#include <stdint.h>

/**
 * \defgroup AUDIO_REPLAYER The API of the libmsx audio replayer.
 * \ingroup AUDIO
 * The API of the libmsx audio replayer.
 *
 * Features of **The libmsx audio replayer** :
 * - Supported sound chip :
 *   - PSG (AY-3-8910) x 1,
 *   - Konami SCC/SCC+ x 1,
 *   - MSX-MUSIC (OPLL/YM2413) x 1.
 *
 * - Pluggable audio decoder design :
 *   - The replayer only provides playback functionality. Loading and/or decoding
 *     music data is performed by supported audio decoders.
 *   - Depending on the supported audio decoders, it can handle various types of
 *     music data.
 *   - If the decoder is compatible, it will support playback of sound effects as
 *     well as background music.
 *
 * - Functionality :
 *   - NTSC (60Hz), PAL/SECAM (50Hz), auto detect and auto adjust.
 *   - Available to specify default playing frequency that the sound data author
 *     expecting, if supported by the decoder.
 *   - Run-time replay frequency control.
 *   - Stop, Start, Pause, Resume.
 *   - Auto-repeat function for background music, if supported by the decoder.
 *   - Playing sound effects (SFX) during playing background music (BGM), if
 *     supported by the decoder.
 *
 * @{
 */

/**
 * `MSX` Initialize the libmsx audio replayer.
 *
 * This function must be called at least once. In particular, it must be called
 * before the first call to audio_play().
 */
void audio_init(void);

/**
 * `MSX` Start / Resume music.
 */
void audio_start(void);

/**
 * `MSX` Pause music.
 */
void audio_pause(void);

/**
 * `MSX` Return whether paused or not.
 *
 * \return `true` if paused.
 */
bool audio_is_paused(void);

/**
 * `MSX` Stop music.
 */
void audio_stop(void);

/**
 * `MSX` Main routine of the libmsx audio replayer.
 *
 * To play back background music and sound effects, this function must be called
 * at each VSYNC timing.
 *
 * The easiest way is to set this function as the VSYNC interrupt handler by
 * calling set_vsync_handler().
 *
 * ~~~c
 * // At first, initialize the replayer.
 * audio_init();
 *
 * // Then, register the replayer as the VSYNC interrupt handler.
 * set_vsync_handler(audio_play);
 *
 * // And then, calls other APIs to start, stop, etc.
 * // \note The replayer is already started in default.
 * // audio_start();
 *
 * // Look up / load a music data and set it to the decoder by using decoder
 * // specific APIs.
 * // (Here, assume that any type or function prefixed with `l2a_` is such a
 * // decoder-specific API.)
 *
 * l2a_File sf;
 * if (!l2a_open_resource(&sf, "songs.l2a") {
 *   // Failed to open l2a file.
 * }
 *
 * l2a_set_bgm(0, &sf);
 * ~~~
 */
void audio_play(void);

/**
 * `MSX` Return replayer frequency of the background music.
 *
 * \return frequency in Hz.
 * \sa audio_set_bgm_frequency()
 */
uint8_t audio_get_bgm_frequency(void);

/**
 * `MSX` Force replayer frequency of the background music.
 *
 * \param freq    frequency in Hz.
 * \sa audio_get_bgm_frequency()
 *
 * \note
 * When a background music is set to the audio decoder, the replayer frequency
 * is set (reset) to the default frequency of the music.
 */
void audio_set_bgm_frequency(uint8_t freq);

/**
 * `MSX` Return whether BGM and/or SFX is playing or not.
 *
 * \return `true` if BGM and/or SFX is playing.
 */
bool audio_is_playing(void);

/**
 * `MSX` Return whether BGM is playing or not.
 *
 * \return `true` if BGM is playing.
 */
bool audio_is_playing_bgm(void);

/**
 * `MSX` Return whether SFX is playing or not.
 *
 * \return `true` if SFX is playing.
 */
bool audio_is_playing_sfx(void);

/**
 * `MSX` Turn on/off the auto-repeat of the BGM.
 *
 * \param repeat `true`: turn on, `false`: turn off
 *
 * \note Valid only if the BGM decoder supports this feature.
 */
void audio_set_repeat(bool repeat);

/** @} */

/**
 * \defgroup AUDIO_EFX Effectors for the libmsx audio replayer.
 * \ingroup AUDIO
 * Effectors for the libmsx audio replayer.
 *
 * @{
 */

/** @} */

#endif // AUDIO_H_
