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

void snd_p_note_on(struct snd_p_ctx * ctx) {
  // // (re)start pitch envelope
  // ctx->timer = 0;
  // ctx->next = ctx->p_table->ad_part;
  (void)ctx;
}

void snd_p_note_off(struct snd_p_ctx * ctx) {
  // // stop pitch envelope
  // ctx->timer = 0;
  // ctx->next = ctx->p_table->r_part;
  (void)ctx;
}

void snd_p__program_change(uint8_t index, struct snd_p_ctx * ctx) {
  // // set pitch envelope table number
  // if (p_number_max < index) {
  //   index = 0;
  // }
  // ctx->p_number = index;
  // ctx->p_table = !index ? &p_table_00 : p_tables[index-1];
  // ctx->wait = ctx->p_table->wait;
  // ctx->timer = 0;
  // ctx->next = ctx->p_table->ad_part;
  (void)index;
  (void)ctx;
}

void snd_p__decode(struct snd_p_ctx * ctx) {
  // if (ctx->timer) {
  //   ctx->timer--;
  //   return;
  // }
  // ctx->timer = ctx->wait;
  // int16_t x = *ctx->next;
  // if (x == END_MARK) {
  //   if (ctx->next < ctx->p_table->r_part) {
  //     ctx->next = ctx->p_table->s_part;
  //     x = *ctx->next;
  //   }
  // }
  // if (x != END_MARK) {
  //   ctx->next++;
  //   ctx->pitch = x;
  // }
  (void)ctx;
}
