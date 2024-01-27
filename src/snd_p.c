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
 * \file snd_p.c
 *
 */

#include "./snd_p.h"

void snd_p_note_on(struct snd_p_ctx * ctx) {
  if (!ctx->p_table) return;
  ctx->timer = 0;
  ctx->next = ctx->p_table->ad_part;
}

void snd_p_note_off(struct snd_p_ctx * ctx) {
  if (!ctx->p_table) return;
  ctx->timer = 0;
  ctx->next = ctx->p_table->r_part;
}

void snd_p__program_change(struct snd_p_ctx * ctx, const struct snd_p_table * p) {
  if (p) {
    ctx->p_table = p;
    ctx->wait = p->wait;
    ctx->timer = 0;
    ctx->next = p->ad_part;
  }
  else {
    ctx->p_table = 0;
    ctx->value = 0;
  }
}

void snd_p__decode(struct snd_p_ctx * ctx) {
  if (!ctx->p_table) {
    return;
  }
  if (ctx->timer) {
    ctx->timer--;
    return;
  }
  ctx->timer = ctx->wait;
  int16_t x = *ctx->next;
  if (!x) {
    if (ctx->next < ctx->p_table->r_part) {
      ctx->next = ctx->p_table->s_part;
      x = *ctx->next;
    }
  }
  if (x) {
    ctx->next++;
    x -= 0x8000;
  }
  ctx->value = x;
}
