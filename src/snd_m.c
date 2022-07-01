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
 * \file snd_m.c
 *
 */

#include "./snd_m.h"

void snd_m__init(struct snd_m_ctx * ctx) {
  snd_m__program_change(ctx, 0);
  struct snd_channel * pch = &ctx->channels[0];
  for (uint8_t ch = 3; ch--; pch++) {
    snd_i__program_change(1, &pch->i); // instrument #1
    snd_channel_reset_expression(pch);
    pch->volume = 0;
    pch->pitch  = -1;
  }
}

void snd_m__program_change(struct snd_m_ctx * ctx, const uint8_t * m_stream) {
  static const uint8_t end_of_stream = 0xff;
  ctx->timer = 0;
  if (!m_stream) {
    ctx->next = ctx->data = &end_of_stream;
    ctx->isEnd = true;
  }
  else {
    ctx->next = ctx->data = m_stream;
    ctx->isEnd = false;
  }
}

static uint8_t snd_m__stream_take(struct snd_m_ctx * ctx);
static void snd_m__decode_expression_command(struct snd_m_ctx * ctx, struct snd_channel * pch);

inline void snd_m__update(struct snd_m_ctx * ctx) {
  struct snd_channel * pch = ctx->channels;
  for (uint8_t ch = 3; ch--; pch++) {
    snd_channel_update(pch);
  }
}

void snd_m__decode(struct snd_m_ctx * ctx) {
  snd_m__update(ctx);
  // ---------------------------------------------------
  if (ctx->timer) {
    ctx->timer--;
    return;
  }
  // ---------------------------------------------------
  for (;;) {
    uint8_t x = snd_m__stream_take(ctx);
    if (x == 0xff) {
      ctx->isEnd = true;
      return;
    }
    if (!(x & 0x80)) {
      ctx->timer = x;
      return;
    }
    // channel message
    const uint8_t ch = x & 0x7f;
    struct snd_channel * pch = &ctx->channels[ch];
    // t_chunk
    x = snd_m__stream_take(ctx);
    if (!x) {
      snd_channel_note_off(pch);
    }
    else if (x & 0x80) {
      // expressions
      uint8_t n = x & 0x0f;
      do {
        snd_m__decode_expression_command(ctx, pch);
      } while (n--);
    }
    else {
      const uint8_t i_number = snd_m__stream_take(ctx);
      snd_channel_note_on(x, i_number, pch);
    }
  }
}

static uint8_t snd_m__stream_take(struct snd_m_ctx * ctx) {
  uint8_t x = *ctx->next;
  if (x != 0xff) {
    ctx->next++;
  }
  return x;
}

static void snd_m__decode_expression_command(struct snd_m_ctx * ctx, struct snd_channel * pch) {
  // decode an expression command
  uint8_t x = snd_m__stream_take(ctx);
  const uint8_t tag = x >> 4;
  x &= 15;
  if (!tag) {
    snd_channel_reset_expression(pch);
    pch->volume = 15 - x;
  }
  else if (tag == 8) {
    // set volume to x
    pch->volume = x;
    pch->fade_speed = 0;
  }
  else {
    const uint16_t xyz = (x << 8) + snd_m__stream_take(ctx);
    if (tag == 1) {
      // arpeggio 3 notes
      // set arp to (+0, +x, +y);
      snd_channel_set_arpeggio(0, 3, xyz, pch);
    }
    else if (tag == 2) {
      // arpeggio 4 notes
      // set arp to (+0, +x, +y, +z);
      snd_channel_set_arpeggio(0, 4, xyz, pch);
    }
    else if (tag == 3) {
      // // pitch up (+0..+4095/128)
      // snd_channel_set_pitch_bend(5, xyz, pch);
      snd_channel_set_period_bend(-1, xyz >> 4, pch);
    }
    else if (tag == 4) {
      // // pitch down (-4095/128..+0)
      // snd_channel_set_pitch_bend(5, -xyz, pch);
      snd_channel_set_period_bend(+1, xyz >> 4, pch);
    }
    else if (tag == 5) {
      // // fast pitch up (+0..+4095/128)
      // snd_channel_set_pitch_bend(0, xyz, pch);
      snd_channel_set_period_bend(-1, xyz, pch);
    }
    else if (tag == 6) {
      // // fast pitch down (-4095/128..+0)
      // snd_channel_set_pitch_bend(0, -xyz, pch);
      snd_channel_set_period_bend(+1, xyz, pch);
    }
    else if (tag == 7) {
      // pitch glide
      pch->pitch_glide = xyz;
    }
    else if (tag == 9) {
      snd_channel_set_fade(+1, xyz, pch);
    }
    else if (tag == 10) {
      snd_channel_set_fade(-1, xyz, pch);
    }
    else if (tag == 11) {
      // force the speed of instrument
      pch->i.wait = xyz >> 4;
    }
    else if (tag == 12) {
      // force the speed of arpeggio
      pch->a.wait = xyz >> 4;
    }
    else if (tag == 13) {
      // force the speed of pitch
      pch->p.wait = pch->pitch_wait = xyz >> 4;
    }
    else if (tag == 14) {
      snd_a__program_change(xyz >> 4, &pch->a);
    }
    else if (tag == 15) {
      snd_p__program_change(xyz >> 4, &pch->p);
    }
  }
}
