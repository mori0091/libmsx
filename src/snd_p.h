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

#include "../include/snd_p_table.h"

struct snd_p_ctx {
  uint8_t wait;
  uint8_t timer;
  uint8_t p_number;                   // pitch envelope table number
  const struct snd_p_table * p_table; // pointer to current instrument table
  const int16_t * next;               // pointer to next lilne of table
  struct {
    int16_t pitch;              // +/- pitch (-32767..+32767)
  };
};

extern void snd_p__set_p_tables(uint8_t n, const struct snd_p_table ** p_tables);

extern void snd_p_note_on(struct snd_p_ctx * ctx);
extern void snd_p_note_off(struct snd_p_ctx * ctx);
extern void snd_p__program_change(uint8_t index, struct snd_p_ctx * ctx);

extern void snd_p__decode(struct snd_p_ctx * ctx);

#endif // SND_P_H_
