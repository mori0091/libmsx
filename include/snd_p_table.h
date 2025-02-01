// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2025 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file snd_p_table.h
 * \brief Pitch-bend table data type for SNDDRV.
 * \ingroup SNDDRV
 *
 * \sa snddrv.h
 * \sa snd_sound.h
 */

#ifndef SND_P_TABLE_H_
#define SND_P_TABLE_H_

#include <stdint.h>

/**
 * \addtogroup SNDDRV_DATA
 * @{
 */

/**
 * Type alias for arpeggio/pitch-bend table.
 */
typedef struct snd_p_table snd_PitchBend;
/**
 * Type alias for wave-length/period modification table.
 */
typedef struct snd_p_table snd_PeriodBend;

/**
 * Pitch bend table.
 *
 * A pitch bend table defines a time series data of a pitch envelope such as
 * pitch slide (pitch bend), vibrato, arpeggio.
 *
 * An pitch bend table contains 3 part of data streams:
 * - `ad_part`:
 *   **Attack** to **Decay** phase.\n
 *   That starts on `NoteOn` message.
 * - `s_part` :
 *   **Sustain** phase. (optional)\n
 *   That starts after `ad_part` and loopbacks until on `NoteOff` message.
 * - `r_part` :
 *   **Release** phase. (optional)\n
 *   That starts on `NoteOff` message.
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

/** @} */

#endif // SND_P_TABLE_H_
