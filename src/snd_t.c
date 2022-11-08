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
 * \file snd_t.c
 *
 */

#include "snd_t.h"
#include <stdint.h>

void snd_t_program_change(const snd_Track * track, int16_t detune, struct snd_t_ctx * ctx) {
  ctx->skip_count = 0;
  ctx->track = track;
  ctx->next = track->data;
  ctx->detune = detune;
}

uint8_t snd_t_stream_take(struct snd_t_ctx * ctx) {
  if (ctx->next < ctx->track->data + ctx->track->length) {
    return *ctx->next++;
  }
  return 0xff;
}
