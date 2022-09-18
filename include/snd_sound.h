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
 * \file snd_sound.h
 * Sound / Music data structure for `snddrv`.
 */

#ifndef SND_SOUND_H_
#define SND_SOUND_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/**
 * A music stream.
 *
 * A `snd_Stream` object represents a non-structured single sound stream.
 * - Pros. :\n
 *   The replayer can read octets one by one until it reaches end of stream.\n
 *   (faster to process)
 * - Cons. : \n
 *   If the music was long or complex, the size of stream may be large.\n
 *   (larger memory consumption)
 *
 * So it is used for container of a sound effect, typically.
 */
typedef struct snd_Stream     snd_Stream;

/**
 * A music program.
 *
 * A `snd_Program` object represents a structured music data.
 * - Pros. :\n
 *   Can compact long or complex multiple pieces of music.
 * - Cons. :\n
 *   Overhead in terms of processing time and memory space.
 *
 * So it is used for container of a background music, typically.
 *
 * A `snd_Program` contains:
 * - one music item,
 * - all tracks refered in the music item, and
 * - all special tracks refered in the music item.
 */
typedef struct snd_Program    snd_Program;

/**
 * A fragment of single channel music score.
 */
typedef struct snd_Track      snd_Track;

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
typedef struct snd_Pattern    snd_Pattern;

/**
 * Define a type generic vector (as anonymous struct).
 * \param T   typename
 *
 * \note
 * THIS MACRO IS INTERNAL USE ONLY and it will be undefined at the last of
 * `snd_sound.h`.
 */
#define vec(T)                                      \
  struct {                                          \
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
  const uint8_t length;                             \
  /** pointer to an array of `const T` */           \
  const T * data

/**
 * A music stream.
 *
 * A `snd_Stream` object represents a non-structured single sound stream.
 * - Pros. :\n
 *   The replayer can read octets one by one until it reaches end of stream.\n
 *   (faster to process)
 * - Cons. : \n
 *   If the music was long or complex, the size of stream may be large.\n
 *   (larger memory consumption)
 *
 * So it is used for container of a sound effect, typically.
 *
 * Ex. Defining a music stream `music` as of snd_Stream type.
 * ~~~c
 * static const uint8_t stream[] = {
 *   // stream data...
 *   0xff, // end mark
 * };
 * const snd_Stream music = {
 *   .replayRate = 60,  // [Hz]
 *   .stream = stream,
 * };
 * ~~~
 *
 * \todo Write documents for defining of music stream.
 */
struct snd_Stream {
  /**
   * Version number of this data format to be used for future compatibility.
   *
   * \note Currently not used, the `format_version` must be `0`.
   */
  const uint8_t       format_version;
  /**
   * Default player frequency that composer expecting, in [Hz].
   */
  const uint8_t       replayRate;
  // ---------------------------------------------------------------------
  /**
   * Pointer to a stream data that ends with 0xff.
   */
  const uint8_t * const stream;
};

/**
 * A music program.
 *
 * A `snd_Program` object represents a structured music data.
 * - Pros. :\n
 *   Can compact long or complex multiple pieces of music.
 * - Cons. :\n
 *   Overhead in terms of processing time and memory space.
 *
 * So it is used for container of a background music, typically.
 *
 * A `snd_Program` contains:
 * - one music item,
 * - all tracks refered in the music item, and
 * - all special tracks refered in the music item.
 *
 * Ex. Defining a music program `music` as of snd_Program type.
 * ~~~c
 * static const snd_Program music = {
 *   .replayRate = 60,    // [Hz]
 *   .speedTracks = {0},  // no speed tracks
 *   .eventTracks = {0},  // no event tracks
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
 * \todo Write documents for defining of music program.
 */
struct snd_Program {
  /**
   * Version number of this data format to be used for future compatibility.
   *
   * \note Currently not used, the `format_version` must be `0`.
   */
  const uint8_t       format_version;
  /**
   * Default player frequency that composer expecting, in [Hz].
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
  uint8_t height;             ///< Number of effective lines of track(s).
  struct {
    uint8_t speedTrack;       ///< Index to the speed track, or `0` if not used.
    uint8_t eventTrack;       ///< Index to the event track, or `0` if not used.
  } specialChannels;
  struct {
    uint8_t track;              ///< Index to the track, or `0` if not used.
    int16_t detune;             ///< Pitch shift [1/256 semi-note]
  } channels[3];
};

#undef vec
#undef vec_fields

#endif // SND_SOUND_H_
