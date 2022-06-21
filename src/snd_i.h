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
 * \file snd_i.h
 *
 */

#ifndef SND_I_H_
#define SND_I_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

struct snd_i_ctx {
  uint8_t wait;
  uint8_t timer;
  uint8_t i_number;                   // instrument table number
  const struct snd_i_table * i_table; // pointer to current instrument table
  const uint8_t * next;               // pointer to next lilne of table
  struct {
    // ____vvvv0
    uint8_t volume;
    // nnnnnt01
    bool    tone_on;            // default: true
    uint8_t noise_fdr;          // default: 0 (off)
    // wwmmT011
    bool    retrig;             // default: false;
    uint8_t modulation;         // default: 0 (SW only)
    uint8_t waveform;
    // _rrr0111
    uint8_t ratio;              // default: 4
    // _pa01111
    uint16_t sw_period;         // default: 0 (auto)
    uint8_t  sw_arp;            // default: 0
    uint16_t sw_pitch;          // default: 0
    // pa011111
    uint16_t hw_period;         // default: 0 (auto)
    uint8_t  hw_arp;            // default: 0
    uint16_t hw_pitch;          // default: 0
  };
};

/**
 * Instrument table.
 */
struct snd_i_table {
  const uint8_t wait;
  const uint8_t * ad_part;
  const uint8_t * s_part;
  const uint8_t * r_part;
};

extern void snd_i__set_i_tables(size_t n, const struct snd_i_table * i_tables_);

extern void snd_i_note_on(struct snd_i_ctx * ctx);
extern void snd_i_note_off(struct snd_i_ctx * ctx);
extern void snd_i__program_change(struct snd_i_ctx * ctx, uint8_t index);

extern uint8_t snd_i__stream_take(struct snd_i_ctx * ctx);
extern void snd_i__decode(struct snd_i_ctx * ctx);

#endif // SND_I_H_
