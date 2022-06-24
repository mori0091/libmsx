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
 * \file snd_p.c
 *
 */

#include "./snd_p.h"

#define END_MARK   (-32768)

static const int16_t p_table_00_data[] = {
  END_MARK,
};
static const struct snd_p_table p_table_00 = {
  .wait    = 255,
  .ad_part = p_table_00_data + 0,
  .s_part  = p_table_00_data + 0,
  .r_part  = p_table_00_data + 0,
};

static const struct snd_p_table * p_tables;
static size_t p_number_max;

void snd_p__set_p_tables(size_t n, const struct snd_p_table * p_tables_) {
  if (!n || !p_tables_) {
    p_tables = 0;
    p_number_max = 0;
  }
  else {
    p_number_max = n;
    p_tables = p_tables_;
  }
}

void snd_p_note_on(struct snd_p_ctx * ctx) {
  // (re)start pitch envelope
  ctx->timer = 0;
  ctx->next = ctx->p_table->ad_part;
}

void snd_p_note_off(struct snd_p_ctx * ctx) {
  // stop pitch envelope
  ctx->timer = 0;
  ctx->next = ctx->p_table->r_part;
}

void snd_p__program_change(struct snd_p_ctx * ctx, uint8_t index) {
  // set pitch envelope table number
  if (p_number_max < index) {
    index = 0;
  }
  ctx->p_number = index;
  ctx->p_table = !index ? &p_table_00 : &p_tables[index-1];
  ctx->wait = ctx->p_table->wait;
  ctx->timer = 0;
  ctx->next = ctx->p_table->ad_part;
}

void snd_p__decode(struct snd_p_ctx * ctx) {
  if (ctx->timer) {
    ctx->timer--;
    return;
  }
  ctx->timer = ctx->wait;
  int16_t x = *ctx->next;
  if (x == END_MARK) {
    if (ctx->next < ctx->p_table->r_part) {
      ctx->next = ctx->p_table->s_part;
      x = *ctx->next;
    }
  }
  if (x != END_MARK) {
    ctx->next++;
    ctx->pitch = x;
  }
}
