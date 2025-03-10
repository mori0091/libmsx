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
 * \file snd_i.c
 *
 */

#include <stdint.h>

#include "./snd_i.h"

static const uint8_t EMPTY_STREAM[] = {255};
static const snd_Instrument i_table0 = {
  .ad_part = EMPTY_STREAM,
  .s_part  = EMPTY_STREAM,
  .r_part  = EMPTY_STREAM,
};

void snd_i_note_on(struct snd_i_ctx * ctx) {
  // (re)start instrument
  ctx->timer = 0;
  ctx->next = ctx->i_table->ad_part;
}

void snd_i_note_off(struct snd_i_ctx * ctx) {
  // go to release phase
  ctx->timer = 0;
  ctx->next = ctx->i_table->r_part;
}

void snd_i__program_change(struct snd_i_ctx * ctx, const snd_Instrument * inst) {
  if (inst) {
    ctx->i_table = inst;
    ctx->wait = inst->wait;
    ctx->timer = 0;
    ctx->next = inst->ad_part;
  }
  else {
    ctx->i_table = &i_table0;
    ctx->wait = 0;
    ctx->timer = 0;
    ctx->next = i_table0.r_part;
  }
}

static uint8_t snd_i__stream_take(struct snd_i_ctx * ctx) {
  uint8_t x = *ctx->next;
  if (x == 0xff) {
    if (ctx->next < ctx->i_table->r_part) {
      ctx->next = ctx->i_table->s_part;
      x = *ctx->next;
    }
  }
  if (x < 0xff) {
    ctx->next++;
  }
  return x;
}

static uint16_t snd_i__stream_take_u16(struct snd_i_ctx * ctx) {
  /* return (snd_i__stream_take(ctx) << 8) + snd_i__stream_take(ctx); */
  uint8_t x = *ctx->next++;
  uint8_t y = *ctx->next++;
  return (x << 8) + y;
}

static int16_t snd_i__stream_take_i16(struct snd_i_ctx * ctx) {
  return (int16_t)snd_i__stream_take_u16(ctx);
}

static void snd_i__init_state(struct snd_i_ctx * ctx) {
  ctx->volume = 0;
  ctx->tone_on = true;
  ctx->noise_fdr = 0;
  ctx->retrig = false;
  ctx->modulation = 0;
  ctx->waveform = 0;
  ctx->ratio = 0;
  ctx->sw_period = 0;
  ctx->sw_period_delta = 0;
  ctx->sw_pitch = 0;
  ctx->hw_period = 0;
  ctx->hw_period_delta = 0;
  ctx->hw_pitch = 0;
}

void snd_i__decode(struct snd_i_ctx * ctx) {
  if (ctx->timer) {
    ctx->timer--;
    return;
  }
  ctx->timer = ctx->wait;
  uint8_t waveform = ctx->waveform;
  snd_i__init_state(ctx);
  for (;;) {
    uint8_t x = snd_i__stream_take(ctx);
    if (!(x & 1)) {
      x >>= 1;
      ctx->volume = x;
      return;
    }
    x >>= 1;
    if (!(x & 1)) {
      x >>= 1;
      ctx->tone_on = x & 1;
      x >>= 1;
      ctx->noise_fdr = x & 0x1f;
      continue;
    }
    x >>= 1;
    if (!(x & 1)) {
      x >>= 1;
      ctx->modulation = (x & 3) + 1;
      x >>= 2;
      ctx->waveform = (x & 7) + 8;
      if (ctx->waveform != waveform) {
        ctx->retrig = true;
      }
      return;
    }
    x >>= 1;
    if (!(x & 1)) {
      x >>= 1;
      ctx->retrig = x & 1;
      x >>= 1;
      ctx->ratio = x & 7;
      continue;
    }
    x >>= 1;
    if (!(x & 1)) {
      x >>= 1;
      if (!(x & 3)) {
        ctx->sw_period = snd_i__stream_take_u16(ctx);
        continue;
      }
      if (x & 1) {
        ctx->sw_period_delta = snd_i__stream_take_i16(ctx);
      }
      if (x & 2) {
        ctx->sw_pitch = snd_i__stream_take_i16(ctx);
      }
      continue;
    }
    x >>= 1;
    if (!(x & 1)) {
      x >>= 1;
      if (!(x & 3)) {
        ctx->hw_period = snd_i__stream_take_u16(ctx);
        continue;
      }
      if (x & 1) {
        ctx->hw_period_delta = snd_i__stream_take_i16(ctx);
      }
      if (x & 2) {
        ctx->hw_pitch = snd_i__stream_take_i16(ctx);
      }
      continue;
    }
    x >>= 1;
    if (!(x & 1)) {
      // _0111111
      continue;
    }
    x >>= 1;
    if (!(x & 1)) {
      // 01111111
      continue;
    }
    return;
  }
}
