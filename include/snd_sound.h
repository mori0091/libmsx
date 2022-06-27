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
 * A tag for `snd_Sound`, that indicates it contains a single sound stream.
 *
 * A `snd_Sound` object of type `SND_STREAM` represents a non-structured single
 * sound stream.
 * - Pros. :\n
 *   The replayer can read octets one by one until it reaches end of stream.\n
 *   (faster to process)
 * - Cons. : \n
 *   If the music was long or complex, the size of stream may be large.\n
 *   (larger memory consumption)
 *
 * So it is used for container of a sound effect, typically.
 */
#define SND_STREAM     (0)

/**
 * A tag for `snd_Sound`, that indicates it contains one or more structured
 * music data.
 *
 * A `snd_Sound` object of type `SND_PROGRAM` represents one or more structured music data.
 * - Pros. :\n
 *   Can compact long or complex multiple pieces of music.
 * - Cons. :\n
 *   Overhead in terms of processing time and memory space.
 *
 * So it is used for container of a background music or a collection of
 * background musics, typically.
 */
#define SND_PROGRAM    (1)

/**
 * Container of a stream data of audio clip or a music program.
 */
typedef struct snd_Sound      snd_Sound;

/**
 * A music program.
 *
 * A `snd_Program` contains:
 * - one or more music items,
 * - all tracks refered in items, and
 * - all special tracks refered in items.
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
 * Corresponds to full score of a music item.
 *
 * A `snd_Matrix` defines which track is played on which channel in which order.
 */
typedef struct snd_Matrix     snd_Matrix;

/**
 * A music item of a program.
 *
 * A `snd_Item` refers to a `snd_Matrix` that corresponds to full score,
 * and it defines:
 * - replay rate (default player frequency that composer expecting),
 * - loop or not, and
 * - initial speed (default speed).
 */
typedef struct snd_Item       snd_Item;

#define vec(T)                                      \
  struct { const uint8_t length; const T * data; }

struct snd_Sound {
  /** `SND_STREAM` or `SND_PROGRAM` */
  const uint8_t tag;
  /**
   * Default frequency of the stream or a program item. [Hz]
   *
   * If not specified (i.e. `replayRate == 0`), the VSYNC frequency is assumed.
   *
   * \note
   * When `tag == SND_PROGRAM`, program items' `replayRate` is prioritised if it
   * is specified.
   */
  const uint8_t replayRate;
  union {
    /** Available if `tag == SND_STREAM` */
    struct {
      /** Pointer to a stream data that ends with 0xff. */
      const uint8_t * const data;
    } stream;
    /** Available if `tag == SND_PROGRAM` */
    struct {
      /** Pointer to a `snd_Program` */
      const snd_Program * const program;
    };
  };
};

struct snd_Program {
  vec(snd_SpeedTrack) speedTracks; ///< [optional] list of all speed tracks refered in items.
  vec(snd_EventTrack) eventTracks; ///< [optional] list of all event tracks refered in items.
  vec(snd_Track)      tracks;      ///< list of all tracks refered in items.
  vec(snd_Item)       items;       ///< at least one item
};

struct snd_Track {
  vec(uint8_t);                 ///< fragment of a single channel music stream
};

struct snd_SpeedTrack {
  vec(uint8_t);                 ///< list of wait counts
};

struct snd_EventTrack {
  vec(uint8_t);                 ///< list of event numbers
};

struct snd_Item {
  const uint8_t    replayRate;   ///< default frequency of the program [Hz]
  const uint8_t    loopToIndex;  ///< pattern index of the beggining of loop.
  const uint8_t    isLoop;       ///< `true` if loopback to the `loopToIndex` at the end of music.
  const uint8_t    initialSpeed; ///< default wait count per line. [tick]
  const snd_Matrix * const matrix; ///< list of patterns. (pattern sequence)
};

struct snd_Matrix {
  /**
   * Size of the matrix in bytes.
   * */
  const size_t  length;

  /**
   * MxN matrix - a series of N dimensions pattern vector.
   * where
   * - M = number of pattern vectors (length / N)
   * - N = 1 + number of tracks
   *
   * A pattern vector defines which track is played on which channel.
   *
   * For each pattern index `i`, an element (i,j) of the matrix is:
   * - (i,0): number of lines of track (i.e. height of tracks)
   * - (i,j): a track number, a speed track number, or a event trac number.
   *
   * 'number of tracks' and the order of track-numbers are determined by
   * `track_bits*`; `track_bits*` is a bitmap of which track/channel is used.
   *
   * For example, if `track_bits0 = 00000101b` and `track_bitsS = 00000001b`,
   * that means PSG channel A, C, and speed track are used. So in that case, an
   * element of the matrix shall be as follows:\n
   * for each pattern index `i`
   * - (i,0): number of lines of track (i.e. height of tracks)
   * - (i,1): a track number for PSG channel A
   * - (i,2): a track number for PSG channel C
   * - (i,3): a speed track number
   */
  const uint8_t * data;

  /**
   * Matrix format identifier.
   *
   * The `format` shall be 0.\n
   * Any other value is reserved for future extension.
   */
  const uint8_t format;

  /**
   * Bit mask #0 to determin which track/channel is used or not used.
   * - bit #0: PSG channel A
   * - bit #1: PSG channel B
   * - bit #2: PSG channel C
   * - bit #3: 0 (reserved)
   * - bit #4: 0 (reserved)
   * - bit #5: 0 (reserved)
   * - bit #6: 0 (reserved)
   * - bit #7: 0 (reserved)
   */
  const uint8_t track_bits0;

  /**
   * Bit mask #1 to determin which track/channel is used or not used.
   * - bit #0: 0 (reserved)
   * - bit #1: 0 (reserved)
   * - bit #2: 0 (reserved)
   * - bit #3: 0 (reserved)
   * - bit #4: 0 (reserved)
   * - bit #5: 0 (reserved)
   * - bit #6: 0 (reserved)
   * - bit #7: 0 (reserved)
   */
  const uint8_t track_bits1;

  /**
   * Bit mask #2 to determin which track/channel is used or not used.
   * - bit #0: 0 (reserved)
   * - bit #1: 0 (reserved)
   * - bit #2: 0 (reserved)
   * - bit #3: 0 (reserved)
   * - bit #4: 0 (reserved)
   * - bit #5: 0 (reserved)
   * - bit #6: 0 (reserved)
   * - bit #7: 0 (reserved)
   */
  const uint8_t track_bits2;

  /**
   * Bit mask for special tracks to determine which special track is used or not
   * used.
   * - bit #0: speed track
   * - bit #1: event track
   * - bit #2: 0 (reserved)
   * - bit #3: 0 (reserved)
   * - bit #4: 0 (reserved)
   * - bit #5: 0 (reserved)
   * - bit #6: 0 (reserved)
   * - bit #7: 0 (reserved)
   */
  const uint8_t track_bitsS;
};

#undef vec

#endif // SND_SOUND_H_
