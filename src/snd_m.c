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
    snd_i__program_change(&pch->i, NULL);
    snd_channel_reset_expression(pch);
    pch->volume = 15;
    pch->pitch  = -1;
  }
}

static void snd_m__set_Pattern(struct snd_m_ctx * ctx, const snd_Music * pg, const snd_Pattern * p);

void snd_m__program_change(struct snd_m_ctx * ctx, const snd_Music * pg) {
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

static void snd_m__set_Pattern(struct snd_m_ctx * ctx, const snd_Music * pg, const snd_Pattern * p) {
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
  ctx->line = 0;
}

static void snd_m__update(struct snd_m_ctx * ctx);
void snd_m__decode_channel(struct snd_channel * pch);
void snd_m__decode_expression_command(struct snd_channel * pch);

static inline void snd_m__update(struct snd_m_ctx * ctx) {
  struct snd_channel * pch = ctx->channels;
  snd_channel_update(pch++);
  snd_channel_update(pch++);
  snd_channel_update(pch);
}

static struct snd_m_ctx * m_ctx;

void snd_m__decode(struct snd_m_ctx * ctx) {
  snd_m__update(ctx);
  // ---------------------------------------------------
  if (ctx->timer) {
    ctx->timer--;
    return;
  }
  // ---------------------------------------------------
  {
    const snd_Music * pg = ctx->music;
    const snd_Pattern * p = &pg->patterns.data[ctx->pindex];
    if (p->height <= ctx->line) {
      // Proceed to the next pattern
      ctx->pindex++;
      p++;
      if (pg->patterns.length <= ctx->pindex) {
        // End of the list of patterns
        if (!ctx->repeat) {
          // finish the program
          ctx->isEnd = true;
          return;
        }
        if (!pg->isLoop) {
          // Loop back to the first pattern
          ctx->pindex = 0;
        }
        else {
          // Loop back to the specified pattern
          // assert(pg->loopToIndex < pg->patterns.length);
          ctx->pindex = pg->loopToIndex;
        }
        p = &pg->patterns.data[ctx->pindex];
      }
      snd_m__set_Pattern(ctx, pg, p);
    }
  }
  // ----
  {
    m_ctx = ctx;
    struct snd_channel * pch = &ctx->channels[0];
    snd_m__decode_channel(pch++);
    snd_m__decode_channel(pch++);
    snd_m__decode_channel(pch);
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

void snd_m__decode_channel(struct snd_channel * pch) {
  if (pch->t.skip_count) {
    pch->t.skip_count--;
    return;
  }
  // t_chunk
  for (;;) {
    const uint8_t x = snd_t_stream_take(&pch->t);
    // note command
    if (!x) {                   // x == 00000000b
      // NoteOff
      snd_channel_note_off(pch);
    }
    else if (!(x & 0x80)) {     // x == 0*******b
      // NoteOn i# (or Legato)
      const uint8_t i_number = snd_t_stream_take(&pch->t);
      const snd_Instrument * inst = 0;
      if (i_number) {
        // assert(i_number <= m_ctx->sa->instruments.length);
        inst = &m_ctx->sa->instruments.data[i_number - 1];
      }
      snd_channel_note_on(x, inst, pch);
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
      snd_i__program_change(&pch->i, NULL);
    }
    else {                      // x == 1111****b
      return;
    }
  }
}

typedef void (* expression_command_func)(uint8_t x, struct snd_channel * pch);

static void expression_command_0(uint8_t x, struct snd_channel * pch) {
  snd_channel_reset_expression(pch);
  pch->volume = 15 - x;
}
static void expression_command_1(uint8_t x, struct snd_channel * pch) {
  const uint16_t xyz = (x << 8) + snd_t_stream_take(&pch->t);
  // arpeggio 3 notes
  // set arp to (+0, +x, +y);
  snd_channel_set_arpeggio(0, 3, xyz, pch);
}
static void expression_command_2(uint8_t x, struct snd_channel * pch) {
  const uint16_t xyz = (x << 8) + snd_t_stream_take(&pch->t);
  // arpeggio 4 notes
  // set arp to (+0, +x, +y, +z);
  snd_channel_set_arpeggio(0, 4, xyz, pch);
}
static void expression_command_3(uint8_t x, struct snd_channel * pch) {
  const uint16_t xyz = (x << 8) + snd_t_stream_take(&pch->t);
  // pitch up (+0..+4095/128)
  // snd_channel_set_pitch_bend(5, xyz, pch);
  snd_channel_set_period_bend(-1, xyz >> 4, pch);
}
static void expression_command_4(uint8_t x, struct snd_channel * pch) {
  const uint16_t xyz = (x << 8) + snd_t_stream_take(&pch->t);
  // pitch down (-4095/128..+0)
  // snd_channel_set_pitch_bend(5, -xyz, pch);
  snd_channel_set_period_bend(+1, xyz >> 4, pch);
}
static void expression_command_5(uint8_t x, struct snd_channel * pch) {
  const uint16_t xyz = (x << 8) + snd_t_stream_take(&pch->t);
  // fast pitch up (+0..+4095/128)
  // snd_channel_set_pitch_bend(0, xyz, pch);
  snd_channel_set_period_bend(-1, xyz, pch);
}
static void expression_command_6(uint8_t x, struct snd_channel * pch) {
  const uint16_t xyz = (x << 8) + snd_t_stream_take(&pch->t);
  // fast pitch down (-4095/128..+0)
  // snd_channel_set_pitch_bend(0, -xyz, pch);
  snd_channel_set_period_bend(+1, xyz, pch);
}
static void expression_command_7(uint8_t x, struct snd_channel * pch) {
  const uint16_t xyz = (x << 8) + snd_t_stream_take(&pch->t);
  // pitch glide
  pch->pitch_glide = xyz;
}
static void expression_command_8(uint8_t x, struct snd_channel * pch) {
  // set volume to x
  pch->volume = x;
  pch->fade_speed = 0;
}
static void expression_command_9(uint8_t x, struct snd_channel * pch) {
  const uint16_t xyz = (x << 8) + snd_t_stream_take(&pch->t);
  snd_channel_set_fade(+1, xyz, pch);
}
static void expression_command_10(uint8_t x, struct snd_channel * pch) {
  const uint16_t xyz = (x << 8) + snd_t_stream_take(&pch->t);
  snd_channel_set_fade(-1, xyz, pch);
}
static void expression_command_11(uint8_t x, struct snd_channel * pch) {
  // const uint16_t xyz = (x << 8) + snd_t_stream_take(&pch->t);
  // const uint8_t xy = xyz >> 4;
  const uint8_t xy = (x << 4) + (snd_t_stream_take(&pch->t) >> 4);
  // force the speed of instrument
  pch->i.wait = xy;
}
static void expression_command_12(uint8_t x, struct snd_channel * pch) {
  // const uint16_t xyz = (x << 8) + snd_t_stream_take(&pch->t);
  // const uint8_t xy = xyz >> 4;
  const uint8_t xy = (x << 4) + (snd_t_stream_take(&pch->t) >> 4);
  // force the speed of arpeggio
  pch->a.wait = xy;
}
static void expression_command_13(uint8_t x, struct snd_channel * pch) {
  // const uint16_t xyz = (x << 8) + snd_t_stream_take(&pch->t);
  // const uint8_t xy = xyz >> 4;
  const uint8_t xy = (x << 4) + (snd_t_stream_take(&pch->t) >> 4);
  // force the speed of pitch
  pch->p.wait = pch->pitch_wait = xy;
}
static void expression_command_14(uint8_t x, struct snd_channel * pch) {
  // const uint16_t xyz = (x << 8) + snd_t_stream_take(&pch->t);
  // const uint8_t idx = xyz >> 4;
  const uint8_t idx = (x << 4) + (snd_t_stream_take(&pch->t) >> 4);
  const snd_PitchBend * p = 0;
  if (idx && idx <= m_ctx->sa->pitchBendTables.length) {
    p = &m_ctx->sa->pitchBendTables.data[idx - 1];
  }
  snd_a__program_change(&pch->a, p);
}
static void expression_command_15(uint8_t x, struct snd_channel * pch) {
  // const uint16_t xyz = (x << 8) + snd_t_stream_take(&pch->t);
  // const uint8_t idx = xyz >> 4;
  const uint8_t idx = (x << 4) + (snd_t_stream_take(&pch->t) >> 4);
  const snd_PeriodBend * p = 0;
  if (idx && idx <= m_ctx->sa->periodBendTables.length) {
    p = &m_ctx->sa->periodBendTables.data[idx - 1];
  }
  snd_p__program_change(&pch->p, p);
}

static const expression_command_func expression_commands[] = {
  expression_command_0,
  expression_command_1,
  expression_command_2,
  expression_command_3,
  expression_command_4,
  expression_command_5,
  expression_command_6,
  expression_command_7,
  expression_command_8,
  expression_command_9,
  expression_command_10,
  expression_command_11,
  expression_command_12,
  expression_command_13,
  expression_command_14,
  expression_command_15,
};

void snd_m__decode_expression_command(struct snd_channel * pch) {
  // decode an expression command
  const uint8_t x = snd_t_stream_take(&pch->t);
  const uint8_t tag = x >> 4;
  expression_commands[tag](x & 15, pch);
}
