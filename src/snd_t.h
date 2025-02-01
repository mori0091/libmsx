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
 * \file snd_t.h
 *
 */

#ifndef SND_T_H_
#define SND_T_H_

#include <stdint.h>
#include <stdbool.h>

#include "../include/snd_sound.h"

struct snd_t_ctx {
  uint8_t skip_count;           // number of subsequent lines to skip
  const snd_Track * track;      // pointer to a track
  const uint8_t * next;         // pointer to the next octet of the track data stream.
  int16_t detune;
};

extern void snd_t_program_change(const snd_Track * track, int16_t detune, struct snd_t_ctx * ctx);
extern uint8_t snd_t_stream_take(struct snd_t_ctx * ctx);

#endif // SND_T_H_
