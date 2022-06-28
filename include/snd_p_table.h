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
 * \file snd_p_table.h
 *
 */

#ifndef SND_P_TABLE_H_
#define SND_P_TABLE_H_

#include <stdint.h>

/**
 * Pitch bend table.
 *
 * A pitch bend table defines a time series data of a pitch envelope such as
 * pitch slide (pitch bend), vibrato.
 *
 * An pitch bend table contains 3 part of data streams:
 * - `ad_part`:
 *   **Attack** to **Decay** phase.\n
 *   That starts on `NoteOn` message.
 * - `s_part` :
 *   **Sustain** phase. (optional)\n
 *   That starts after `ad_part` and loopbacks until on `NoteOff` message.
 * - `r_part` :
 *   Release phase. (optional)\n
 *   That starts on `NoteOff` message.
 *
 * \todo Write documents for defining of pitch bend data stream.
 */
struct snd_p_table {
  /**
   * Wait counts (ticks per data chunk).
   *
   * This determines the speed to process the pitch bend data stream:
   * - if `wait = 0`, data is processed every ticks.
   * - if `wait = 1`, data is processed every **2** ticks.
   * - if `wait = n`, data is processed every **n+1** ticks.
   */
  const uint8_t wait;
  /** Pointer to Atack..Decay part of data stream. */
  const int16_t * ad_part;
  /** Pointer to Sustain part of data stream. (loop part) */
  const int16_t * s_part;
  /** Pointer to Release part of data stream. */
  const int16_t * r_part;
};

/**
 * Register pitch bend tables.
 *
 * \param n         number of pitch bend tables.
 * \param p_tables  array of pointers to pitch bend tables.
 */
void snd_p_set_p_tables(uint8_t n, const struct snd_p_table ** p_tables);

#endif // SND_P_TABLE_H_
