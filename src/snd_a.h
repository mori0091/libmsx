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
 * \file snd_a.h
 *
 */

#ifndef SND_A_H_
#define SND_A_H_

#include <stdint.h>
#include <stdbool.h>

struct snd_a_ctx {
  uint8_t timer;
  uint8_t a_number[3];          // arpeggio table number
};

extern void snd_a_note_on(struct snd_a_ctx * ctx);
extern void snd_a_note_off(struct snd_a_ctx * ctx);
extern void snd_a__program_change(struct snd_a_ctx * ctx, uint8_t index);
extern void snd_a__decode(struct snd_a_ctx * ctx);

#endif // SND_A_H_
