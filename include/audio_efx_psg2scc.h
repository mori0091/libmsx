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
 * \file audio_efx_psg2scc.h
 * \brief The "PSG to SCC/SCC+" effector.
 * \ingroup AUDIO_EFX
 */

#ifndef AUDIO_EFX_PSG2SCC_H_
#define AUDIO_EFX_PSG2SCC_H_

/**
 * \defgroup AUDIO_EFX_PSG2SCC The "PSG to SCC/SCC+" effector.
 * \ingroup AUDIO_EFX
 *
 * `#include <audio_efx_psg2scc.h>`
 *
 * The "PSG to SCC/SCC+" effector copies the pitch and volume of the PSG
 * channels to the SCC/SCC+.
 *
 * It is useful for creating PSG and SCC/SCC+ unisons from a PSG-only sound
 * source.
 *
 * ~~~ c
 * #include <msx.h>
 * #include <audio.h>
 * #include <audio_efx_psg2scc.h>
 *
 * // Custom VSYNC interrupt handler.
 * void play(void) {
 *   audio_play();
 *   // To use the effector, call `audio_efx_psg2scc()` every after `audio_play()`.
 *   audio_efx_psg2scc();
 * }
 *
 * void main(void) {
 *   audio_init();
 *   set_vsync_handler(play);
 *   for (;;) {
 *     await_vsync();
 *     // ...
 *   }
 * }
 * ~~~
 * @{
 */

/**
 * `MSX` The "PSG to SCC/SCC+" effector for the libmsx audio replayer.
 *
 * Calling this function each time after `audio_play()` will copies the pitch
 * and volume of the PSG channels to the SCC/SCC+.
 */
void audio_efx_psg2scc(void);

/** @} */

#endif // AUDIO_EFX_PSG2SCC_H_
