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
 * \file snd_e.h
 *
 */

#ifndef SND_E_H_
#define SND_E_H_

#include <stdint.h>
#include <stdbool.h>

struct snd_e_ctx {
  uint8_t wait;
  uint8_t timer;
  uint8_t e_number;             // amplitude envelope table number
};

extern void snd_e_note_on(struct snd_e_ctx * ctx);
extern void snd_e_note_off(struct snd_e_ctx * ctx);
extern void snd_e__program_change(struct snd_e_ctx * ctx, uint8_t index);
extern void snd_e__decode(struct snd_e_ctx * ctx);

#endif // SND_E_H_
