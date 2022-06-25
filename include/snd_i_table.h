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
 * \file snd_i_table.h
 *
 */

#ifndef SND_I_TABLE_H_
#define SND_I_TABLE_H_

#include <stdint.h>
#include <stddef.h>

/**
 * Instrument (timbre) table.
 *
 * An instrument table defines a time series data of an instrument's sound.
 */
struct snd_i_table {
  /** Wait counts (ticks per data chunk) */
  const uint8_t wait;
  /** Pointer to Atack..Decay part of data stream. */
  const uint8_t * ad_part;
  /** Pointer to Sustain part of data stream. (loop part) */
  const uint8_t * s_part;
  /** Pointer to Release part of data stream. */
  const uint8_t * r_part;
};

/**
 * Register instrument (timbre) tables.
 *
 * \param n         number of instrument tables.
 * \param i_tables  array of pointers to instrument tables.
 */
void snd_set_i_tables(size_t n, const struct snd_i_table ** i_tables);

#endif // SND_I_TABLE_H_
