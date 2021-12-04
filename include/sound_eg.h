// -*- coding: utf-8-unix -*-
/**
 * \file sound_eg.h
 *
 * Copyright (c) 2021 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

#ifndef SOUND_EG_H
#define SOUND_EG_H

#pragma once

#include <stdbool.h>
#include <stdint.h>

/** The maximum number of elements for the envelope generator attribute table. */
#define SOUND_EG_TABLE_MAX   (16)

/** The class identifier for AHDSR envelope generator */
#define SOUND_EG_AHDSR       (0)
/** The class identifier for LPCM envelope generator */
#define SOUND_EG_LPCM        (1)

/** Parameters for AHDSR envelope generator */
struct sound_eg_AHDSR {
  /** Asccendant velocity of attack phase. */
  uint8_t attack_rate;
  /** Period of hold phase. */
  uint8_t hold_time;
  /** Deccendant velocity of decay phase. */
  uint8_t decay_rate;
  /** Output level of sustain phase. */
  uint8_t sustain_level;
  /** desccendant velocity of release phase */
  uint8_t release_rate;
};

/** Parameters for linear PCM (8-bit, 60Hz) envelope generator */
struct sound_eg_LPCM {
  /** number of sampled signal levels */
  uint8_t len;
  /** time series of the sampled signal levels (i.e. wave form) */
  uint8_t * data;
};

/**
 * Attributes of a software envelope generator object.
 *
 * The attribute consists of the following two parts:
 * 1. an envelope generator class identifier `clz`
 * 2. a set of parameter constants `param`
 *
 * The `clz` shall be one of the two classes:
 * - `SOUND_EG_AHDSR` for AHDSR type envelope generator, or
 * - `SOUND_EG_LPCM` for 8-bit 60Hz Linear PCM type envelope generator.
 *
 * The `param` shall be a pointer to parameter constants, and it has the
 * following two strictly typed sinonym:
 * - `ahdsr` of `const struct sound_eg_AHDSR *` type for AHDSR type envelope
 *   generator.
 * - `lpcm` of `const struct sound_eg_LPCM *` type for 8-bit 60Hz Linear PCM
 *   type envelope generator.
 */
struct sound_eg_attribute {
  /**
   * The class identifier of this envelope generator.
   */
  uint8_t clz;
  union {
    /**
     * Pointer to parameter constants for this instance.
     */
    void * param;
    /**
     * Pointer to parameter constants for this instance of the SOUND_EG_AHDSR
     * class.
     *
     * This is a strictly typed sinonym for `param`.
     */
    const struct sound_eg_AHDSR * ahdsr;
    /**
     * Pointer to parameter constants for this instance of the SOUND_EG_LPCM
     * class.
     *
     * This is a strictly typed sinonym for `param`.
     */
    const struct sound_eg_LPCM * lpcm;
  };
};

/**
 * (Re)set the software envelope generator attribute table.
 *
 * This function sets the base address of the software envelope generator table
 * to the given address `table`, or reset to the default if `table` was `NULL`.
 *
 * The table should be an array of `struct sound_eg_attribute` with a maximum of
 * 16 elements. Each element can contain the individual parameter settings of an
 * envelope generator of type AHDSR or LPCM.
 *
 * \param table    base address of the attribute table.
 */
void sound_set_eg_table(const struct sound_eg_attribute * table);

#endif
