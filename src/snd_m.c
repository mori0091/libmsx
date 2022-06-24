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

/**
 * Multiply an 8 bit coefficient `k` to `x`.
 *
 * \param k  an 8 bit coefficient (0..255 means 0..255/256)
 * \param x  a 16 bit unsigned integer
 * \return   the integer part of k*x
 */
inline uint16_t kxQ8(uint8_t k, uint16_t x) {
  uint16_t h = (k * (x >> 8));
  uint16_t l = (k * (x & 255)) >> 8;
  return (h + l);
}

/**
 * Linear interpolation between `a` and `b`.
 *
 * \param k  an 8 bit coefficient (0..255 means 0..255/256)
 * \param x  a 16 bit unsigned integer
 * \return   the integer part of the result.
 * \note
 * `k` must be less than or equal to 128.
 */
inline uint16_t lerpQ8(uint8_t k, uint16_t a, uint16_t b) {
  return !k ? a : (kxQ8(256 - k, a) + kxQ8(k, b));
}

/**
 * \param pitch   pitch (pitch == 128 means +1 note)
 * \return        OSC period (≈ wave lengh)
 */
static int16_t snd__osc_period(int16_t pitch) {
  if (pitch < 0) {
    // out of range (illegal MIDI note number)
    return osc_periods[0];
  }
  if ((127 << 8) < pitch) {
    // out of range (illegal MIDI note number)
    return osc_periods[OSC_PERIODS_INDEX_MAX - 1];
  }
  int8_t idx = MIDI_NOTE_NUMBER_TO_INDEX(pitch >> 8);
  if (OSC_PERIODS_INDEX_MAX - 1 <= idx) {
    return osc_periods[OSC_PERIODS_INDEX_MAX - 1];
  }
  return lerpQ8(pitch & 255, osc_periods[idx], osc_periods[idx+1]);
}

static void reset_effect(struct snd_channel * pch) {
  snd_a__program_change(&pch->a, 0); // arpeggio off
  snd_p__program_change(&pch->p, 0); // pitch envelope off
  snd_e__program_change(&pch->e, 0); // amplitude envelope off
  pch->arp   = 0;
  pch->fade  = 0;
}

void snd_m__init(struct snd_m_ctx * ctx) {
  snd_m__program_change(ctx, 0);
  for (uint8_t ch = 3; ch--;) {
    struct snd_channel * pch = &ctx->channels[ch];
    snd_i__program_change(&pch->i, 1); // instrument #1
    reset_effect(pch);
    pch->volume = 0;
    pch->pitch  = -1;
  }
}

void snd_m__program_change(struct snd_m_ctx * ctx, const uint8_t * m_stream) {
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

uint8_t snd_m__stream_take(struct snd_m_ctx * ctx) {
  // \TODO
  uint8_t x = *ctx->next;
  if (x != 0xff) {
    ctx->next++;
  }
  return x;
}

static void snd_m__decode_expression_command(struct snd_m_ctx * ctx, struct snd_channel * pch) {
  // decode an expression command
  const uint8_t x = snd_m__stream_take(ctx);
  const uint8_t tag = x >> 4;
  if (!tag) {
    // \TODO reset effect
    reset_effect(pch);
    pch->volume = ~x & 15;
  }
  else if (tag == 8) {
    // set volume to x
    pch->volume = x & 15;
    pch->fade = 0;
  }
  else {
    const uint16_t xyz = ((x & 15) << 8) + snd_m__stream_take(ctx);
    if (tag == 1) {
      // \TODO arpeggio 3 notes
      // set arp to (+0, +x, +y);
      return;
    }
    else if (tag == 2) {
      // \TODO arpeggio 4 notes
      // set arp to (+0, +x, +y, +z);
      return;
    }
    else if (tag == 3) {
      // \TODO pitch up (+0..+4095/128)
      // set speed of slow pitch slide
      return;
    }
    else if (tag == 4) {
      // \TODO pitch down (-4095/128..+0)
      return;
    }
    else if (tag == 5) {
      // \TODO fast pitch up (+0..+4095/128)
      return;
    }
    else if (tag == 6) {
      // \TODO fast pitch down (-4095/128..+0)
      return;
    }
    else if (tag == 7) {
      // \TODO pitch glide
      return;
    }
    else if (tag == 9) {
      pch->fade = 1;
      pch->fade_wait = pch->fade_timer = xyz;
    }
    else if (tag == 10) {
      pch->fade = -1;
      pch->fade_wait = pch->fade_timer = xyz;
    }
    else if (tag == 11) {
      // \TODO force the speed of instrument
      pch->i.wait = xyz >> 4;
    }
    else if (tag == 12) {
      // \TODO force the speed of arpeggio
      pch->a.wait = xyz >> 4;
    }
    else if (tag == 13) {
      // \TODO force the speed of pitch
      pch->p.wait = xyz >> 4;
    }
    // else if (tag == 14) {
    //   // (reserved)
    // }
    // else if (tag == 15) {
    //   // (reserved)
    // }
  }
}

static void snd_m__update_fade_in_out(struct snd_channel * pch) {
  if (!pch->fade) {
    return;
  }
  if (pch->fade_timer) {
    pch->fade_timer--;
    return;
  }
  pch->fade_timer = pch->fade_wait;
  int8_t amp = pch->volume + pch->fade;
  if (amp <= 0 || 15 <= amp) {
    pch->fade = 0;
  }
  else {
    pch->volume = (uint8_t)amp;
  }
}

static void snd_m__update(struct snd_m_ctx * ctx) {
  struct snd_channel * pch = ctx->channels;
  for (uint8_t ch = 3; ch--; pch++) {
    snd_m__update_fade_in_out(pch);
  }
}

void snd_m__decode(struct snd_m_ctx * ctx) {
  snd_m__update(ctx);
  // ---------------------------------------------------
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
    struct snd_channel * pch = &ctx->channels[x & 0x0f];
    switch (x >> 4) {
      case 8:                   // NoteOn
        pch->p.pitch = 0;
        pch->fade = 0;
        uint8_t note = snd_m__stream_take(ctx);
        if (note < 0x80) {
          pch->pitch = note << 8;
          snd_a_note_on(&pch->a);
          snd_i_note_on(&pch->i);
          snd_p_note_on(&pch->p);
          snd_e_note_on(&pch->e);
        }
        // legato
        else {
          pch->pitch = (note & 0x7f) << 8;
        }
        break;
      case 9:
        pch->pitch = -1;
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
        snd_m__decode_expression_command(ctx, pch);
        break;
      case 15:
      default:
        break;
    }
  }
}

void snd_m__synthesis(struct snd_channel * pchs[3]) {
  uint8_t mixer = 0xb8;
  for (uint8_t ch = 3; ch--; ) {
    struct snd_channel * pch = pchs[ch];
    if (pch->pitch < 0) {
      PSG_SET(ch+8, 0);
      continue;
    }
    // ----
    if (pch->i.noise_fdr) {
      PSG_SET(6, pch->i.noise_fdr);
      mixer &= ~(4 << ch);
    }
    if (!pch->i.tone_on) {
      mixer |= (1 << ch);
    }
    // ----
    int16_t pitch = pch->pitch + 256 * pch->arp + pch->p.pitch;
    // ----
    uint16_t sw_period = pch->i.sw_period;
    // SW only -----------------------------------------
    if (pch->i.modulation == 0) {
      // ---- volume / software envelope ----
      int8_t volume = pch->volume + pch->i.volume - 15;
      if (volume < 0)  { volume = 0;  }
      if (15 < volume) { volume = 15; }
      PSG_SET(ch+8, volume);
      // ---- square wave ----
      if (!sw_period) {
        sw_period = snd__osc_period(pitch + 256 * pch->i.sw_arp + pch->i.sw_pitch);
      }
      PSG_SET(2*ch+0, (sw_period     ) & 0xff);
      PSG_SET(2*ch+1, (sw_period >> 8) & 0xff);
      continue;
    }
    // ----
    uint16_t hw_period = pch->i.hw_period;
    // HW only -----------------------------------------
    if (pch->i.modulation == 1) {
      mixer |= (1 << ch);
      if (!hw_period) {
        hw_period = snd__osc_period(pitch + 256 * pch->i.hw_arp + pch->i.hw_pitch) >> pch->i.ratio;
      }
    }
    else {
      mixer &= ~(1 << ch);
      // SW -> HW ----------------------------------------
      if (pch->i.modulation == 2) {
        if (!sw_period) {
          sw_period = snd__osc_period(pitch + 256 * pch->i.sw_arp + pch->i.sw_pitch);
        }
        if (!hw_period) {
          hw_period = sw_period >> pch->i.ratio;
        }
      }
      // HW -> SW ----------------------------------------
      else if (pch->i.modulation == 3) {
        if (!hw_period) {
          hw_period = snd__osc_period(pitch + 256 * pch->i.hw_arp + pch->i.hw_pitch) >> pch->i.ratio;
        }
        if (!sw_period) {
          sw_period = hw_period << pch->i.ratio;
        }
      }
      // SW + HW -----------------------------------------
      else if (pch->i.modulation == 4) {
        if (!hw_period) {
          hw_period = snd__osc_period(pitch + 256 * pch->i.hw_arp + pch->i.hw_pitch) >> pch->i.ratio;
        }
        if (!sw_period) {
          sw_period = snd__osc_period(pitch + 256 * pch->i.sw_arp + pch->i.sw_pitch);
        }
      }
      // ??      -----------------------------------------
      else {
        // nothing to do
      }
      PSG_SET(2*ch+0, (sw_period     ) & 0xff);
      PSG_SET(2*ch+1, (sw_period >> 8) & 0xff);
    }
    PSG_SET(11, (hw_period     ) & 0xff);
    PSG_SET(12, (hw_period >> 8) & 0xff);
    if (pch->i.retrig) {
      // R13 = 8:Saw, 10:Triangle, 12:Inv-Saw, 14:Inv-Triangle
      PSG_SET(13, pch->i.waveform * 2 + 8);
    }
    PSG_SET(8+ch, 16);
  }
  PSG_SET(7, mixer);
}
