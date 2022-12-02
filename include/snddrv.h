// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2022 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file snddrv.h
 * \brief SNDDRV a PSG sound driver.
 * \ingroup SNDDRV
 */
/**
 * \defgroup SNDDRV SNDDRV - PSG sound driver
 * `#include <snddrv.h>`
 *
 * SNDDRV is a new PSG sound driver inspired by [Arkos Tracker
 * 2](http://www.julien-nevo.com/arkostracker/).
 *
 * SNDDRV covers almost feature of replayer of the Arkos Tracker 2.
 *
 * SNDDRV and libmsx's build system make it easy to integrate Arkos Tracker 2
 * song file (.aks file) into your application.
 *
 * Let's make your music with Arkos Tracker 2. Place its .aks files in your
 * application's repository, and build together with your application code that
 * uses SNDDRV API.
 *
 * See also
 * - [Arkos Tracker 2](http://www.julien-nevo.com/arkostracker/).
 * - [snddrv_demo](https://github.com/mori0091/libmsx/tree/main/sample/snddrv_demo) the sample project.
 * - libmsx API reference
 *  - [snddrv.h](https://mori0091.github.io/libmsx/snddrv_8h.html)
 *  - [snd_sound.h](https://mori0091.github.io/libmsx/snd__sound_8h.html)
 * - "Build instructions of user applications" section of libmsx's
 *   [README.md](https://github.com/mori0091/libmsx)
 * - [DESIGN NOTE: SNDDRV - a sound driver](https://github.com/mori0091/libmsx/blob/main/docs/design-notes_snddrv.md)
 * - [DESIGN NOTE: Internal of SNDDRV](https://github.com/mori0091/libmsx/blob/main/docs/design-notes_snddrv_internal.md)
 */

#ifndef SNDDRV_H_
#define SNDDRV_H_

#include "./snd_sound.h"
#include <stdint.h>

/**
 * \defgroup SNDDRV_API SNDDRV APIs
 * \ingroup SNDDRV
 * SNDDRV APIs.
 * @{
 */

/**
 * The value of the playback speed multiplier for 1.0x speed.
 * \sa snd_set_speed()
 */
#define SND_SPEED_1X      (16)

/**
 * `MSX` Returns default frequency of the background music.
 *
 * \return frequency in Hz.
 * \sa snd_get_player_frequency()
 * \sa snd_set_player_frequency()
 * \sa snd_set_speed()
 */
uint8_t snd_get_bgm_frequency(void);

/**
 * `MSX` Returns current player frequency of the background music.
 *
 * \return frequency in Hz.
 * \sa snd_get_bgm_frequency()
 * \sa snd_set_player_frequency()
 * \sa snd_set_speed()
 */
uint8_t snd_get_player_frequency(void);

/**
 * `MSX` Force player frequency of the background music.
 *
 * \param freq    frequency in Hz.
 * \sa snd_get_bgm_frequency()
 * \sa snd_get_player_frequency()
 * \sa snd_set_speed()
 *
 * \note
 * When a music is set by snd_set_bgm(), the player frequency is calculated
 * regarding to the default frequency of the music and the speed set by
 * snd_set_speed().
 *
 * \note
 * Default frequency that the music composer assumed may not be the same.
 * Therefore snd_set_speed() is recommended instead.
 */
void snd_set_player_frequency(uint8_t freq);

/**
 * `MSX` Sets the playback speed multiplier for background music.
 *
 * This function is used to control the playback speed of background music.
 *
 * The playback speed multiplier is specified by a value of 16 times. For
 * example, a value of 16 means a playback speed of 1.0x, and a value of 24
 * means 1.5x. (i.e. Speed multiplier values are fixed-point numbers with a
 * 4-bit integer part and a 4-bit decimal part.)
 *
 * ~~~c
 * snd_set_speed(SND_SPEED_1X * 2);  // 2.0x faster playback
 * snd_set_speed(SND_SPEED_1X / 4);  // 1/4x slower playback
 * ~~~
 *
 * \param multiplier    the playback speed multiplier
 * \sa SND_SPEED_1X
 * \sa snd_get_bgm_frequency()
 * \sa snd_get_player_frequency()
 * \sa snd_set_player_frequency()
 */
void snd_set_speed(uint8_t multiplier);

/**
 * `MSX` Turn on/off the auto-repeat of the BGM.
 *
 * \param repeat   `true`: turn on, `false`: turn off
 */
void snd_set_repeat(bool repeat);

/**
 * `MSX` Sets the specified music as BGM in the sound driver.
 *
 * \param index Number of a music to be played as BGM.
 * \param sa    Pointer to the sound assets that contains the music.
 */
void snd_set_bgm(uint8_t index, const snd_SoundAssets * sa);

/**
 * `MSX` Sets the specified music as SFX (sound effects) in the sound driver.
 *
 * \param index Number of a music to be played as SFX.
 * \param sa    Pointer to the sound assets that contains the music.
 */
void snd_set_sfx(uint8_t index, const snd_SoundAssets * sa);

/**
 * `MSX` Initialize the PSG and the sound driver.
 *
 * This function must be called at least once. In particular, it must be called
 * before the first call to snd_play().
 *
 * \note
 * If snd_play() is called without snd_init() being called, its behavior is
 * undefined and, in the worst case, may cause damage to the (real) MSX machine.
 */
void snd_init(void);

/**
 * `MSX` Start / Resume music.
 */
void snd_start(void);

/**
 * `MSX` Pause music.
 */
void snd_pause(void);

/**
 * `MSX` Stop music.
 */
void snd_stop(void);

/**
 * `MSX` Main routine of the sound driver.
 *
 * To play the background music and/or sound effects, you need to call this
 * function at every VSYNC timing. The easiest way is to set this function as
 * the VSYNC interrupt handler by calling set_vsync_handler().
 *
 * ~~~c
 * // At first, initialize the sound driver.
 * snd_init();
 *
 * // Then, register the sound driver as the VSYNC interrupt handler.
 * set_vsync_handler(snd_play);
 *
 * // And then, calls other APIs to start, stop, etc.
 * snd_set_bgm(0, &my_bgm);
 * snd_start();
 * ~~~
 *
 * \note
 * If snd_play() is called without snd_init() being called, its behavior is
 * undefined and, in the worst case, may cause damage to the (real) MSX machine.
 */
void snd_play(void);

/** @} */

#endif // SNDDRV_H_
