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

struct snd_p_ctx {
  uint8_t timer;
  uint8_t p_number[3];          // pitch envelope table number
};

extern void snd_p_note_on(struct snd_p_ctx * ctx);
extern void snd_p_note_off(struct snd_p_ctx * ctx);
extern void snd_p__program_change(struct snd_p_ctx * ctx, uint8_t index);
extern void snd_p__decode(struct snd_p_ctx * ctx);

#endif // SND_P_H_
