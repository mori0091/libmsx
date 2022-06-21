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
 * \file snd_i.c
 *
 */

#include "../include/ay_3_8910.h"

#include "./snd_m.h"
#include "./snd_i.h"
#include "./snd_a.h"
#include "./snd_p.h"
#include "./snd_e.h"

#define PSG_SET(reg, val)    ay_3_8910_buffer[(reg)] = (val)

#define MIDI_NOTE_NUMBER_C4          (60)
#define OSC_PERIODS_INDEX_C4         (3+3*12)
#define OSC_PERIODS_INDEX_MAX        (sizeof(osc_periods) / sizeof(osc_periods[0]))
#define MIDI_NOTE_NUMBER_TO_INDEX(x) ((x) + OSC_PERIODS_INDEX_C4 - MIDI_NOTE_NUMBER_C4)

static const uint8_t end_of_stream = 0xff;

static const uint16_t osc_periods[] = {
  // C      C#     D      D#     E      F      F#     G      G#     A      A#     B
                                                                 0xfe4, 0xf00, 0xe28, // octave 0
  0xd5d, 0xc9c, 0xbe7, 0xb3c, 0xa9b, 0xa02, 0x973, 0x8eb, 0x86b, 0x7f2, 0x780, 0x714, // octave 1
  0x6af, 0x64e, 0x5f4, 0x59e, 0x54e, 0x501, 0x4ba, 0x476, 0x436, 0x3f9, 0x3c0, 0x38a, // octave 2
  0x357, 0x327, 0x2fa, 0x2cf, 0x2a7, 0x281, 0x25d, 0x23b, 0x21b, 0x1fd, 0x1e0, 0x1c5, // octave 3
  0x1ac, 0x194, 0x17d, 0x168, 0x153, 0x140, 0x12e, 0x11d, 0x10d, 0x0fe, 0x0f0, 0x0e3, // octave 4
  0x0d6, 0x0ca, 0x0be, 0x0b4, 0x0aa, 0x0a0, 0x097, 0x08f, 0x087, 0x07f, 0x078, 0x071, // octave 5
  0x06b, 0x065, 0x05f, 0x05a, 0x055, 0x050, 0x04c, 0x047, 0x043, 0x040, 0x03c, 0x039, // octave 6
  0x035, 0x032, 0x030, 0x02d, 0x02a, 0x028, 0x026, 0x024, 0x022, 0x020, 0x01e, 0x01c, // octave 7
  0x01b, 0x019, 0x018, 0x016, 0x015, 0x014, 0x013, 0x012, 0x011, 0x010, 0x00f, 0x00e, // octave 8
  0x00e, 0x00d, 0x00c, 0x00b, 0x00b, 0x00a, 0x00a, 0x009, 0x009, 0x008, 0x008, 0x007, // octave 9
};

inline int16_t snd__osc_period_of(uint8_t note) {
  int8_t idx = MIDI_NOTE_NUMBER_TO_INDEX(note);
  if (idx < 0 || OSC_PERIODS_INDEX_MAX <= idx) {
    // out of range
    return -1;
  }
  return osc_periods[idx];
}

void snd_m__init(struct snd_m_ctx * ctx) {
  snd_m__program_change(ctx, 0);
  ctx->isEnd = true;
  ctx->timer = 0;
  snd_i__set_i_tables(0, 0);
  for (uint8_t ch = 0; ch < 3; ++ch) {
    struct snd_channel * pch = &ctx->channels[ch];
    snd_i__program_change(&pch->i, 1); // instrument #1
    snd_a__program_change(&pch->a, 0); // arpeggio off
    snd_p__program_change(&pch->p, 0); // pitch envelope off
    snd_e__program_change(&pch->e, 0); // amplitude envelope off
    pch->note = 0xff;
    pch->volume = 15;
    pch->arp = 0;
    pch->pitch = 0;
    pch->volume = 0;
    pch->fade_delta = 0;
    pch->fade_timer = 0;
    pch->fade_wait = 0;
    pch->period = 0;
  }
}

void snd_m__program_change(struct snd_m_ctx * ctx, const uint8_t * m_stream) {
  if (!m_stream) {
    ctx->next = ctx->data = &end_of_stream;
    ctx->isEnd = true;
    return;
  }
  ctx->next = ctx->data = m_stream;
  ctx->isEnd = false;
}

uint8_t snd_m__stream_take(struct snd_m_ctx * ctx) {
  // \TODO
  uint8_t x = *ctx->next;
  if (x != 0xff) {
    ctx->next++;
  }
  return x;
}

void snd_m__decode_expression_command(struct snd_m_ctx * ctx, uint8_t ch) {
  // decode an expression command
  uint8_t tag = snd_m__stream_take(ctx);
  uint8_t x = tag & 0x0f;
  struct snd_channel * pch = &ctx->channels[ch];
  switch (tag >> 4) {
    case 0:
      // \TODO reset effect
      pch->volume = 15 - x;
      break;
    case 1:
      // \TODO arpeggio 2 notes
      break;
    case 2:
      // \TODO arpeggio 3 notes
      snd_m__stream_take(ctx);
      break;
    case 3:
      // \TODO arpeggio 4 notes
      snd_m__stream_take(ctx);
      break;
    case 4:
      // \TODO pitch up
      break;
    case 5:
      // \TODO pitch down
      break;
    case 6:
      // \TODO fast pitch up
      snd_m__stream_take(ctx);
      break;
    case 7:
      // \TODO fast pitch down
      snd_m__stream_take(ctx);
      break;
    case 8:
      // \TODO pitch glide
      snd_m__stream_take(ctx);
      break;
    case 9:
      // set volume to x
      pch->volume = x;
      break;
    case 10:
      // \TODO fade in
      pch->fade_delta = 1;
      pch->fade_wait
        = pch->fade_timer
        = (x << 8) + snd_m__stream_take(ctx);
      break;
    case 11:
      // \TODO fade out
      pch->fade_delta = -1;
      pch->fade_wait
        = pch->fade_timer
        = (x << 8) + snd_m__stream_take(ctx);
      break;
    case 12:
      // \TODO force the speed of
      // - instrument (x == 0)
      // - arpeggio   (x == 1)
      // - pitch      (x == 2)
      snd_m__stream_take(ctx);
      break;
    default:
      // illegal expression command
      break;
  }
}

void snd_m__decode(struct snd_m_ctx * ctx) {
  if (ctx->timer) {
    ctx->timer--;
    return;
  }
  for (;;) {
    uint8_t x = snd_m__stream_take(ctx);
    if (x == 0xff) {
      ctx->isEnd = true;
      return;
    }
    if (x < 0x80) {
      ctx->timer = x;
      return;
    }
    uint8_t ch = x & 0x0f;
    struct snd_channel * pch = &ctx->channels[ch];
    switch (x >> 4) {
      case 8:                   // NoteOn
        pch->note = snd_m__stream_take(ctx);
        snd_a_note_on(&pch->a);
        snd_i_note_on(&pch->i);
        snd_p_note_on(&pch->p);
        snd_e_note_on(&pch->e);
        break;
      case 9:
        pch->note = 0xff;
        snd_a_note_off(&pch->a);
        snd_i_note_off(&pch->i);
        snd_p_note_off(&pch->p);
        snd_e_note_off(&pch->e);
        break;
      case 10:
        snd_a__program_change(&pch->a, snd_m__stream_take(ctx));
        break;
      case 11:
        snd_p__program_change(&pch->p, snd_m__stream_take(ctx));
        break;
      case 12:
        snd_i__program_change(&pch->i, snd_m__stream_take(ctx));
        break;
      case 13:
        snd_e__program_change(&pch->e, snd_m__stream_take(ctx));
        break;
      case 14:
        snd_m__decode_expression_command(ctx, ch);
        break;
      case 15:
      default:
        break;
    }
  }
}

static void snd_m__synthesis_fade_in_out(struct snd_m_ctx * ctx) {
  uint8_t ch = 3;
  while (ch--) {
    struct snd_channel * pch = &ctx->channels[ch];
    int8_t fade = pch->fade_delta;
    if (fade) {
      if (pch->fade_timer) {
        pch->fade_timer--;
        continue;
      }
      pch->fade_timer = pch->fade_wait;
      int8_t amp = pch->volume + fade;
      if (amp < 0) {
        amp = 0;
        pch->fade_delta = 0;
        continue;
      }
      if (15 < amp) {
        amp = 15;
        pch->fade_delta = 0;
        continue;
      }
      pch->volume = (uint8_t)amp;
    }
  }
}

static void snd_m__synthesis_pitch(struct snd_m_ctx * ctx) {
  uint8_t ch = 3;
  while (ch--) {
    struct snd_channel * pch = &ctx->channels[ch];
    int16_t period = snd__osc_period_of(pch->note + pch->arp);
    if (period < 0) {
      pch->period = -1;
      continue;
    }
    int16_t pitch = pch->pitch;
    if (0 < pitch && period < pitch) {
      pch->period = 1;
      continue;
    }
    period -= pitch;
    if (0x0fff < period) {
      pch->period = 0x0fff;
      continue;
    }
    pch->period = period;
  }
}

void snd_m__synthesis(struct snd_m_ctx * ctx) {
  snd_m__synthesis_fade_in_out(ctx);
  snd_m__synthesis_pitch(ctx);
}

void snd_m__mixing(struct snd_m_ctx * ctx, uint8_t ch) {
  struct snd_channel * pch = &ctx->channels[ch];
  int16_t period = pch->period;
  if (period <= 0) {
    PSG_SET(ch+8, 0);
    return;
  }
  int8_t volume = pch->volume + pch->i.volume - 15;
  if (volume < 0) {
    volume = 0;
  }
  if (15 < volume) {
    volume = 15;
  }
  PSG_SET(2*ch+0, period & 0xff);
  PSG_SET(2*ch+1, (period >> 8));
  PSG_SET(  ch+8, volume);
  // PSG_SET(6, 0);                // noise
  // PSG_SET(7, 0xb8);             // mixer
  // PSG_SET(11, 0);               // hw envelope cycle (lo)
  // PSG_SET(12, 0);               // hw envelope cycle (hi)
  // PSG_SET(13, 0xff);            // set hw envelope curve & reset phase
}
