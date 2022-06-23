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
 * \file snd_p.h
 *
 */

#ifndef SND_P_H_
#define SND_P_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

struct snd_p_ctx {
  uint8_t wait;
  uint8_t timer;
  uint8_t p_number;                   // pitch envelope table number
  const struct snd_p_table * p_table; // pointer to current instrument table
  const uint8_t * next;               // pointer to next lilne of table
  struct {
    int16_t pitch;              // +/- pitch
  };
};

extern void snd_p_note_on(struct snd_p_ctx * ctx);
extern void snd_p_note_off(struct snd_p_ctx * ctx);
extern void snd_p__program_change(struct snd_p_ctx * ctx, uint8_t index);
extern void snd_p__decode(struct snd_p_ctx * ctx);

/**
 * Pitch envelope table.
 *
 * A pitch envelope table defines a time series data of a pitch envelope such as
 * pitch slide (pitch bend), vibrato.
 */
struct snd_p_table {
  /** Wait counts (ticks per data chunk) */
  const uint8_t wait;
  /** Pointer to Atack..Decay part of data stream. */
  const uint8_t * ad_part;
  /** Pointer to Sustain part of data stream. (loop part) */
  const uint8_t * s_part;
  /** Pointer to Release part of data stream. */
  const uint8_t * r_part;
};

extern void snd_p__set_p_tables(size_t n, const struct snd_p_table * p_tables);

#endif // SND_P_H_
