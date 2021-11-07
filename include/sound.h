// -*- coding: utf-8-unix -*-
/**
 * \file sound.h
 *
 * Copyright (c) 2021 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

#ifndef SOUND_H
#define SOUND_H

#pragma once

#include <stdint.h>
#include <stdbool.h>

/**
 * The bit-mask to specify PSG channel A.
 */
#define SOUND_CHANNEL_A   (1 << 0)

/**
 * The bit-mask to specify PSG channel B.
 */
#define SOUND_CHANNEL_B   (1 << 1)

/**
 * The bit-mask to specify PSG channel C.
 */
#define SOUND_CHANNEL_C   (1 << 2)

/**
 * The bit-mask to specify all PSG channels.
 */
#define SOUND_CHANNEL_ALL (SOUND_CHANNEL_A | SOUND_CHANNEL_B | SOUND_CHANNEL_C)

/**
 * The sound clip structure.
 *
 * The sound clip structure holds a list of pointers to the sound data stream
 * for each channel.
 *
 * This structure also stores the priority order when the sound clip is used as
 * a sound effect. (If the sound clip is to be used as background music, its
 * priority has no meaning and will be ignored.)
 *
 * Each sound data stream shall be in the following format.
 *
 * \note
 * The following is written in BNF-like notation.
 * - The term `x*` means zero or more `x`.
 * - The term `x+` means one or more `x`.
 * - If a term ends with `b` (e.g. `1111b`), it means a binary number.
 * - A `-` in a binary number means an unused bit.
 * - If a term ends with `:y` (e.g. `foo:5`), it means that the term is a binary
 *   number with y bits.
 *
 * ~~~
 * stream = chunk* eom
 * chunk  = header body
 * body   = command+
 * eom    = 1111b 1111b
 *
 * header  = len:3 duration_hi:5 duration_lo:8
 * command = tone
 *         | rest
 *         | volume
 *         | envelope_pattern
 *         | envelope_cycle
 *         | envelope_pattern_and_cycle
 *         | noise
 *         | mixer
 *
 * tone   = 0000b fdr_hi:4 fdr_lo:8
 * rest   = 0001b ----b
 * noise  = 001b fdr:5
 * mixer  = 0111b ----b --b NC:1 NB:1 NA:1 TC:1 TB:1 TA:1
 * volume = 1000b vol:4
 * envelope_pattern
 *        = 1001b pat:4
 * envelope_cycle
 *        = 0100b 0000b fdr_hi:8 fdr_lo:8
 * envelope_pattern_and_cycle
 *        = 1101b pat:4 fdr_hi:8 fdr_lo:8
 * ~~~
 *
 * The structure of `chunk`:
 * ~~~
 * +-----------------------+  -----
 * | len:3 | duration_hi:5 |    |
 * |-----------------------|  header (2 bytes)
 * |     duration_lo:8     |    |
 * |-----------------------|  -----
 * |       body[0]:8       |    |
 * |-----------------------|    |
 * |       body[1]:8       |    |
 * |-----------------------|  body (`len` bytes)
 * |         ...           |    |
 * |-----------------------|    |
 * |     body[len-1]:8     |    |
 * +-----------------------+  -----
 * The body is a sequence of one or more commands.
 * ~~~
 *
 */
struct sound_clip {
  /** A list of pointers to the sound data stream for each channel. */
  uint8_t* streams[3];
  /** Priority for using this sound clip as a sound effect. */
  uint16_t priority;
};

/**
 * Turn on/off the auto-repeat of the BGM.
 *
 * \param repeat   `true`: turn on, `false`: turn off
 */
void sound_set_repeat(bool repeat);

/**
 * Mute/unmute for each sound channel.
 *
 * The parameter `mute` is a bit-set of the mute switches.
 *
 * - bit #0 (SOUND_CHANNEL_A) is for PSG channel A
 * - bit #1 (SOUND_CHANNEL_B) is for PSG channel B
 * - bit #2 (SOUND_CHANNEL_C) is for PSG channel C
 *
 * If a bit is `1`, the corresponding channel is muted;
 * if a bit is `0`, the corresponding channel is unmuted.
 *
 * \param mute   the bit-set of the mute switches.
 */
void sound_set_mute(uint8_t mute);

/**
 * Plays the specified music clip as sound effect.
 *
 * /Priority of the sound effect/
 *
 * The priority of the sound effect is specified by `s->priority`. The larger
 * the value, the higher the priority. This priority determines whether or not
 * the sound driver will play the specified sound clip, as follows
 *
 * - If no sound effect is playing, the specified sound clip will be played
 *   immediately.
 *
 * - If a sound effect is playing and the priority of the specified sound clip
 *   is higher than that, the current sound effect will be stopped and the
 *   specified sound clip will be played immediately.
 *
 * - If a sound effect is playing and the priority of the specified sound clip
 *   is lower than or equal to that, the specified sound clip will be ignored
 *   and will not be played.
 *
 * /Auto-mute of the background music/
 *
 * While the sound effect is playing, some channels of the background music will
 * be automatically muted (if they conflict with the sound effect) and those
 * channels will be used to play the sound effect.
 *
 * When the sound effect finishes playing, all channels will be used for the
 * background music again.
 *
 * \param s Pointer to the music clip structure to be played as sound effect.
 */
void sound_effect(const struct sound_clip* s);

/**
 * Sets the specified music clip as BGM in the sound driver.
 *
 * \param s Pointer to the music clip structure to be played as BGM.
 */
void sound_set_bgm(const struct sound_clip* s);

/**
 * Start the BGM.
 */
void sound_start(void);

/**
 * Stop the BGM.
 */
void sound_stop(void);

/**
 * Pause the BGM.
 */
void sound_pause(void);

/**
 * Main routine of the sound driver.
 *
 * To play the background music, you need to call this function at every VSYNC
 * timing. The easiest way is to set this function as the VSYNC interrupt
 * handler by calling set_vsync_handler().
 *
 * ~~~c
 * // At first, register the sound driver as the VSYNC interrupt handler.
 * set_vsync_handler(sound_player);
 *
 * // then calls other APIs to start, stop, etc.
 * sound_set_repeat(true);
 * sound_set_bgm(&my_bgm);
 * sound_start();
 * ~~~
 */
void sound_player(void);

#endif