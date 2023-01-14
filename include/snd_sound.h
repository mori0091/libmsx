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
 * \file snd_sound.h
 * Sound / Music data structure for `snddrv`.
 * \ingroup SNDDRV
 */

#ifndef SND_SOUND_H_
#define SND_SOUND_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "snd_i_table.h"
#include "snd_p_table.h"

/**
 * \defgroup SNDDRV_DATA Sound data structure
 * \ingroup SNDDRV
 * Sound data structure for SNDDRV.
 * @{
 */

/**
 * Container of musics, instruments, pitch-bend tables, and period-bend tables.
 *
 * A `snd_SoundAssets` object contains:
 * - one or more instruments,
 * - zero or more pitch-bend tables,
 * - zero or more period-bend tables, and
 * - one or more musics that shares and refers the above.
  */
typedef struct snd_SoundAssets snd_SoundAssets;

/**
 * Container of a music data.
 *
 * A `snd_Music` object represents one music.
 *
 * A `snd_Music` object contains:
 * - all tracks refered in the music, and
 * - all special tracks refered in the music item.
 * - a series of patterns that defines which track is played on which channel.
 */
typedef struct snd_Music snd_Music;

/**
 * A fragment of single channel music score.
 */
typedef struct snd_Track snd_Track;

/**
 * A special track to control speed of tracks.
 */
typedef struct snd_SpeedTrack snd_SpeedTrack;

/**
 * A special track to control event trigger of tracks.
 * \note unused yet.
 */
typedef struct snd_EventTrack snd_EventTrack;

/**
 * A track pattern, that corresponds to a sub-score of a music.
 *
 * A `snd_Pattern` defines which track is played on which channel.\n
 * A vector / series of `snd_Pattern`s corresponds to a full-score of a music.
 */
typedef struct snd_Pattern snd_Pattern;

/**
 * Define a type generic vector (as named struct).
 * \param T   typename
 *
 * \note
 * THIS MACRO IS INTERNAL USE ONLY and it will be undefined at the last of
 * `snd_sound.h`.
 */
#define vec(T)                                      \
  struct vec_ ## T {                                \
    vec_fields(T);                                  \
  }

/**
 * Define fields (member variables) of a type generic vector.
 * \param T   typename
 *
 * \note
 * THIS MACRO IS INTERNAL USE ONLY and it will be undefined at the last of
 * `snd_sound.h`.
 */
#define vec_fields(T)                               \
  /** number of elements */                         \
  const size_t length;                              \
  /** pointer to an array of `const T` */           \
  const T * data

/**
 * Container of list of musics, instruments, arpeggio tables, and period tables.
 *
 * A `snd_SoundAssets` object contains:
 * - one or more instruments,
 * - zero or more pitch-bend tables,
 * - zero or more period-bend tables, and
 * - one or more musics that shares and refers the above.
 */
struct snd_SoundAssets {
  vec(snd_PitchBend)  pitchBendTables;
  vec(snd_PeriodBend) periodBendTables;
  vec(snd_Instrument) instruments;
  vec(snd_Music)      musics;
};

/**
 * Container of a music data.
 *
 * A `snd_Music` object represents one music.
 *
 * A `snd_Music` object contains:
 * - all tracks refered in the music, and
 * - all special tracks refered in the music.
 * - a series of patterns that defines which track is played on which channel.
 *
 * Ex. Defining a music `my_music` as of snd_Music type.
 * ~~~c
 * static const snd_Music my_music = {
 *   .replayRate = 60,    // [Hz]
 *   .speedTracks = {
 *     .length = ...,     // number of speed tracks
 *     .data = ...,       // pointer to an array of `snd_SpeedTrack`s
 *   },
 *   .eventTracks = {
 *     .length = ...,     // number of event tracks
 *     .data = ...,       // pointer to an array of `snd_EventTrack`s
 *   },
 *   .tracks = {
 *     .length = ...,     // number of tracks
 *     .data = ...,       // pointer to an array of `snd_Track`s
 *   },
 *   .patterns = {
 *     .length = ...,     // number of track patterns.
 *     .data = ...,       // pointer to an array of `snd_Pattern`s.
 *   },
 *   .initialSpeed = 5,
 *   .isLoop = false,
 *   .loopToIndex = 0,
 * };
 * ~~~
 *
 */
struct snd_Music {
  /**
   * Version number of this data format to be used for future compatibility.
   *
   * \note Currently not used, the `format_version` must be `0`.
   */
  const uint8_t       format_version;
  /**
   * Default player frequency that author expecting, in [Hz].
   */
  const uint8_t       replayRate;
  // ---------------------------------------------------------------------
  /** [optional] list of all speed tracks refered in items. */
  vec(snd_SpeedTrack) speedTracks;
  /** [optional] list of all event tracks refered in items. */
  vec(snd_EventTrack) eventTracks;
  /** list of all tracks refered in items. */
  vec(snd_Track)      tracks;
  /** list of patterns (pattern sequence). */
  vec(snd_Pattern)    patterns;
  /** default wait count per line [tick]. */
  const uint8_t       initialSpeed;
  /** `true` if loopback to the `loopToIndex` at the end of music. */
  const uint8_t       isLoop;
  /** pattern index of the beggining of loop. */
  const uint8_t       loopToIndex;
};

/**
 * A fragment of single channel music score.
 */
struct snd_Track {
  vec_fields(uint8_t);
};

/**
 * A special track to control speed of tracks.
 */
struct snd_SpeedTrack {
  vec_fields(uint8_t);
};

/**
 * A special track to control event trigger of tracks.
 * \note unused yet.
 */
struct snd_EventTrack {
  vec_fields(uint8_t);
};

/**
 * A track pattern, that corresponds to a sub-score of a music.
 *
 * A `snd_Pattern` defines which track is played on which channel.\n
 * A vector / series of `snd_Pattern`s corresponds to a full-score of a music.
 */
struct snd_Pattern {
  uint8_t height;               ///< Number of effective lines of track(s).
  struct {
    uint8_t speedTrack;         ///< Index to the speed track.
    uint8_t eventTrack;         ///< Index to the event track.
  } specialChannels;
  struct {
    uint8_t track;              ///< Index to the track.
    int16_t detune;             ///< Pitch shift [1/256 semi-note]
  } channels[3];
};

#undef vec
#undef vec_fields

/** @} */

#endif // SND_SOUND_H_
