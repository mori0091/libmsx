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
 * \file audio_efx_amp.h
 * \brief The AMP effector.
 * \ingroup AUDIO_EFX
 */
#ifndef AUDIO_EFX_AMP_H_
#define AUDIO_EFX_AMP_H_

#include <stdint.h>

/**
 * \defgroup AUDIO_EFX_AMP The AMP effector.
 * \ingroup AUDIO_EFX
 *
 * `#include <audio_efx_amp.h>`
 *
 * The AMP effector controls / attenuate the main-volume for all channels.
 *
 * ~~~ c
 * #include <msx.h>
 * #include <audio.h>
 * #include <audio_efx_amp.h>
 *
 * // Custom VSYNC interrupt handler.
 * void play(void) {
 *   audio_play();
 *   // To use AMP effector, call `audio_efx_amp()` every after `audio_play()`.
 *   audio_efx_amp();
 * }
 *
 * void main(void) {
 *   audio_init();
 *   set_vsync_handler(play);
 *   for (;;) {
 *     await_vsync();
 *     // Get the current main-volume level.
 *     uint8_t vol = audio_efx_amp_get_volume();
 *     // ...
 *
 *     // Set the main-volume level to 10.
 *     audio_efx_amp_set_volume(10);
 *     // ...
 *
 *     // Decrease the main-volume level every 2 ticks.
 *     audio_efx_amp_fadeout(2);
 *     // ...
 *
 *     // Increase the main-volume level every 2 ticks.
 *     audio_efx_amp_fadein(2);
 *     // ...
 *   }
 * }
 * ~~~
 * @{
 */

/**
 * `MSX` The AMP effector for the libmsx audio replayer.
 *
 * Calling this function each time after `audio_play()` will control or
 * attenuate the main volume of all PSG, SCC/SCC+, and OPLL channels.
 */
void audio_efx_amp(void);

/**
 * `MSX` Sets the main volume level to the AMP effector.
 *
 * \param volume  The main volume level. (0..15)
 */
void audio_efx_amp_set_volume(uint8_t volume);

/**
 * `MSX` Gets the current main volume level from the AMP effector.
 *
 * \return  The current main volume level. (0..15)
 */
uint8_t audio_efx_amp_get_volume(void);

/**
 * `MSX` Decrease the main volume level by one for each specified period of
 * time. (Fade out)
 *
 * \param ticks  Period (VSYNC counts)
 */
void audio_efx_amp_fadeout(uint8_t ticks);

/**
 * `MSX` Increase the main volume level by one for each specified period of
 * time. (Fade in)
 *
 * \param ticks  Period (VSYNC counts)
 */
void audio_efx_amp_fadein(uint8_t ticks);

/** @} */

#endif // AUDIO_EFX_AMP_H_
