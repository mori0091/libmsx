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
#include <stdint.h>

void snd_m__init(struct snd_m_ctx * ctx) {
  snd_m__program_change(ctx, 0);
  // ----
  struct snd_channel * pch = &ctx->channels[0];
  for (uint8_t ch = 3; ch--; pch++) {
    snd_i__program_change(1, &pch->i); // instrument #1
    snd_channel_reset_expression(pch);
    pch->volume = 15;
    pch->pitch  = -1;
  }
}

static void snd_m__set_Pattern(struct snd_m_ctx * ctx, const snd_Program * pg, const snd_Pattern * p);

void snd_m__program_change(struct snd_m_ctx * ctx, const snd_Program * pg) {
  ctx->timer = 0;
  ctx->pindex = 0;
  ctx->line = 0;
  if (!pg) {
    ctx->music = 0;
    ctx->isEnd = true;
  }
  else {
    ctx->music = pg;
    ctx->isEnd = false;
    ctx->wait = pg->initialSpeed;
    snd_m__set_Pattern(ctx, pg, &pg->patterns.data[0]);
  }
}

static void snd_m__set_Pattern(struct snd_m_ctx * ctx, const snd_Program * pg, const snd_Pattern * p) {
  for (uint8_t ch = 3; ch--; ) {
    const int16_t detune = p->channels[ch].detune;
    const uint8_t trackNumber = p->channels[ch].track;
    const snd_Track * track = &pg->tracks.data[trackNumber];
    struct snd_t_ctx * tctx = &ctx->channels[ch].t;
    snd_t_program_change(track, detune, tctx);
  }
  const uint8_t speedTrackNumber = p->specialChannels.speedTrack;
  if (speedTrackNumber < pg->speedTracks.length) {
    const snd_SpeedTrack * speedTrack = &pg->speedTracks.data[speedTrackNumber];
    ctx->spd.next = speedTrack->data;
    ctx->spd.skip_count = 0;
    ctx->timer = 0;
  }
}

static uint8_t snd_m__stream_take(struct snd_m_ctx * ctx);
static void snd_m__decode_channel(struct snd_m_ctx * ctx, struct snd_channel * pch);
static void snd_m__decode_expression_command(struct snd_channel * pch);

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
  const snd_Program * pg = ctx->music;
  const snd_Pattern * p = &pg->patterns.data[ctx->pindex];
  if (p->height <= ctx->line) {
    // Proceed to the next pattern
    ctx->line = 0;
    ctx->pindex++;
    p++;
    if (pg->patterns.length <= ctx->pindex) {
      // End of the list of patterns
      if (pg->isLoop && pg->loopToIndex < pg->patterns.length) {
        // Loop back to the specified pattern
        ctx->pindex = pg->loopToIndex;
        p = &pg->patterns.data[ctx->pindex];
      }
      else {
        // finish the program (unless 'auto-repeat' is set)
        ctx->isEnd = true;
        return;
      }
    }
    snd_m__set_Pattern(ctx, pg, p);
  }
  for (uint8_t ch = 3; ch--; ) {
    struct snd_channel * pch = &ctx->channels[ch];
    struct snd_t_ctx * tctx = &pch->t;
    if (tctx->skip_count) {
      tctx->skip_count--;
    }
    else {
      snd_m__decode_channel(ctx, pch);
    }
  }
  // ----
  if (ctx->spd.next) {
    if (ctx->spd.skip_count) {
      ctx->spd.skip_count--;
    }
    else {
      // decode speed track
      for (;;) {
        uint8_t x = *ctx->spd.next;
        if (x == 0xff) {
          // EOM
          break;
        }
        ctx->spd.next++;
        if (0xc0 <= x) {
          // EOL(skipLines)
          ctx->spd.skip_count = x & 0x3f;
          break;
        }
        else {
          // change speed
          ctx->wait = x;
        }
      }
    }
  }
  ctx->timer = ctx->wait;

  // Proceed to the next line of the current pattern
  ctx->line++;
}

static void snd_m__decode_channel(struct snd_m_ctx * ctx, struct snd_channel * pch) {
  (void)ctx;
  // t_chunk
  for (;;) {
    uint8_t x = snd_t_stream_take(&pch->t);
    // note command
    if (!x) {                   // x == 00000000b
      // NoteOff
      snd_channel_note_off(pch);
    }
    else if (!(x & 0x80)) {     // x == 0*******b
      // NoteOn i# (or Legato)
      const uint8_t i_number = snd_t_stream_take(&pch->t);
      snd_channel_note_on(x, i_number, pch);
    }
    else if (!(x & 0x40)) {     // x == 10******b
      // expressions
      uint8_t n = x & 0x0f;
      do {
        snd_m__decode_expression_command(pch);
      } while (n--);
    }
    else if (!(x & 0x20)) {     // x == 110*****b
      // EOL (End of line and skip lines)
      pch->t.skip_count = x & 0x1f;
      return;
    }
    else if (!(x & 0x10)) {     // x == 1110****b
      // Reset
      snd_i__program_change(0, &pch->i);
    }
    else {                      // x == 1111****b
      return;
    }
  }
}

static void snd_m__decode_expression_command(struct snd_channel * pch) {
  // decode an expression command
  uint8_t x = snd_t_stream_take(&pch->t);
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
    const uint16_t xyz = (x << 8) + snd_t_stream_take(&pch->t);
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
