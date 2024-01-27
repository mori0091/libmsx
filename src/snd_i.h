// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2024 Daishi Mori (mori0091)
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

#include "../include/snd_i_table.h"

struct snd_i_ctx {
  uint8_t wait;
  uint8_t timer;
  const snd_Instrument * i_table; // pointer to the current instrument table
  const uint8_t * next;           // pointer to next lilne of table
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
    int16_t  sw_period_delta;   // default: 0 ; wave length offset
    int16_t  sw_pitch;          // default: 0 ; pitch bend / arpeggio
    // pa011111
    uint16_t hw_period;         // default: 0 (auto)
    int16_t  hw_period_delta;   // default: 0 ; wave length offset
    int16_t  hw_pitch;          // default: 0 ; pitch bend / arpeggio
  };
};

extern void snd_i_note_on(struct snd_i_ctx * ctx);
extern void snd_i_note_off(struct snd_i_ctx * ctx);
extern void snd_i__program_change(struct snd_i_ctx * ctx, const snd_Instrument * inst);

extern void snd_i__decode(struct snd_i_ctx * ctx);

#endif // SND_I_H_
