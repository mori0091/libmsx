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
 * \file snd_channel.c
 *
 */

#include "./snd_channel.h"

#include "../include/ay_3_8910.h"

#define PSG(reg)                     (ay_3_8910_buffer[(reg)])

#define OSC_PERIODS_INDEX_MAX        (sizeof(osc_periods) / sizeof(osc_periods[0]))
#define MIDI_NOTE_NUMBER_MIN         (21) // A0
#define MIDI_NOTE_NUMBER_MAX         (MIDI_NOTE_NUMBER_MIN + OSC_PERIODS_INDEX_MAX - 1)
#define MIDI_NOTE_NUMBER_TO_INDEX(x) ((x) - MIDI_NOTE_NUMBER_MIN)

#define PITCH_MIN                    (MIDI_NOTE_NUMBER_MIN << 8)
#define PITCH_MAX                    (MIDI_NOTE_NUMBER_MAX << 8)

static const uint16_t osc_periods[] = {
  // C      C#     D      D#     E      F      F#     G      G#     A      A#     B
                                                                 0xfe4, 0xf00, 0xe28, // octave 0
  0xd5d, 0xc9c, 0xbe7, 0xb3c, 0xa9b, 0xa02, 0x973, 0x8eb, 0x86b, 0x7f2, 0x780, 0x714, // octave 1 ( #24..)
  0x6af, 0x64e, 0x5f4, 0x59e, 0x54e, 0x501, 0x4ba, 0x476, 0x436, 0x3f9, 0x3c0, 0x38a, // octave 2 ( #36..)
  0x357, 0x327, 0x2fa, 0x2cf, 0x2a7, 0x281, 0x25d, 0x23b, 0x21b, 0x1fd, 0x1e0, 0x1c5, // octave 3 ( #48..)
  0x1ac, 0x194, 0x17d, 0x168, 0x153, 0x140, 0x12e, 0x11d, 0x10d, 0x0fe, 0x0f0, 0x0e3, // octave 4 ( #60..)
  0x0d6, 0x0ca, 0x0be, 0x0b4, 0x0aa, 0x0a0, 0x097, 0x08f, 0x087, 0x07f, 0x078, 0x071, // octave 5 ( #72..)
  0x06b, 0x065, 0x05f, 0x05a, 0x055, 0x050, 0x04c, 0x047, 0x043, 0x040, 0x03c, 0x039, // octave 6 ( #84..)
  0x035, 0x032, 0x030, 0x02d, 0x02a, 0x028, 0x026, 0x024, 0x022, 0x020, 0x01e, 0x01c, // octave 7 ( #96..)
  0x01b, 0x019, 0x018, 0x016, 0x015, 0x014, 0x013, 0x012, 0x011, 0x010, 0x00f, 0x00e, // octave 8 (#108..)
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
 * \param a  a 16 bit unsigned integer
 * \param b  the other 16 bit unsigned integer
 * \return   the integer part of the result.
 */
inline uint16_t lerpQ8(uint8_t k, uint16_t a, uint16_t b) {
  return !k ? a : (kxQ8(256 - k, a) + kxQ8(k, b));
}

/**
 * \param pitch   pitch (pitch == 128 means +1 note)
 * \return        OSC period (≈ wave lengh)
 */
static int16_t snd__osc_period(int16_t pitch) {
  if (pitch < (MIDI_NOTE_NUMBER_MIN << 8)) {
    // out of range (illegal MIDI note number)
    return osc_periods[0];
  }
  if ((MIDI_NOTE_NUMBER_MAX << 8) <= pitch) {
    // out of range (illegal MIDI note number)
    return osc_periods[OSC_PERIODS_INDEX_MAX - 1];
  }
  int8_t idx = MIDI_NOTE_NUMBER_TO_INDEX(pitch >> 8);
  return lerpQ8(pitch & 255, osc_periods[idx], osc_periods[idx+1]);
}

void snd_channel_note_on(uint8_t note, struct snd_channel * pch) {
  if (pch->pitch_triggered) {
    pch->pitch_triggered = false;
  }
  else {
    // Turn pitch-bend off
    snd_p__program_change(0, &pch->p);
    pch->pitch_delta = 0;
  }
  // ----
  if (pch->fade_triggered) {
    pch->fade_triggered = false;
  }
  else {
    // Turn fade-in/out off
    pch->fade = 0;
  }
  // --------------------------------------------------------
  const uint16_t pitch = (note & 0x7f) << 8;
  // ---- Start pitch glide toward to the given note.
  if (0 <= pch->pitch && pch->pitch_glide) {
    // Turn pitch glide on
    if (pitch < pch->pitch) {
      snd_channel_set_pitch_bend(0, -pch->pitch_glide, pch);
      pch->pitch_min = pitch;
    }
    else if (pch->pitch < pitch) {
      snd_channel_set_pitch_bend(0, +pch->pitch_glide, pch);
      pch->pitch_max = pitch;
    }
    pch->pitch_glide = 0;
  }
  else {
    // ---- Legato : switch to the given note w/o attack
    pch->pitch = pitch;
    // ---- Normal NoteOn (attack)
    if (note < 0x80) {
      snd_a_note_on(&pch->a);
      snd_i_note_on(&pch->i);
      snd_p_note_on(&pch->p);
    }
  }
}

void snd_channel_note_off(struct snd_channel * pch) {
  // \note
  // `Note off` means `KEY OFF`.
  // In other words, it means switching to release phase.
  // Thus the pitch should not be cleared.
  // pch->pitch = -1; // <- DO NOT THIS HERE
  snd_a_note_off(&pch->a);
  snd_i_note_off(&pch->i);
  snd_p_note_off(&pch->p);
}

inline void snd_channel_reset_arpeggio(struct snd_channel * pch) {
  snd_a__program_change(0, &pch->a); // arpeggio off
  pch->arp = 0;
  pch->arp_vec_data = 0;
}
inline void snd_channel_reset_pitch_bend(struct snd_channel * pch) {
  snd_p__program_change(0, &pch->p); // pitch envelope off
  pch->pitch_delta = 0;
}
inline void snd_channel_reset_pitch_glide(struct snd_channel * pch) {
  pch->pitch_glide = 0;
  pch->pitch_min = PITCH_MIN;
  pch->pitch_max = PITCH_MAX;
}
inline void snd_channel_reset_fade(struct snd_channel * pch) {
  pch->fade = 0;
}

void snd_channel_reset_expression(struct snd_channel * pch) {
  snd_channel_reset_arpeggio(pch);
  snd_channel_reset_pitch_bend(pch);
  snd_channel_reset_pitch_glide(pch);
  snd_channel_reset_fade(pch);
}

void snd_channel_set_pitch_bend(uint8_t wait, int16_t pitch_delta, struct snd_channel * pch) {
  pch->pitch_wait = wait;
  pch->pitch_timer = 0;
  pch->pitch_delta = pitch_delta;
  pch->pitch_triggered = true;
  pch->pitch_min = PITCH_MIN;
  pch->pitch_max = PITCH_MAX;
}

void snd_channel_set_fade(int8_t fade, uint16_t wait, struct snd_channel * pch) {
  pch->fade_wait = wait;
  pch->fade_timer = 0;
  pch->fade = fade;
  pch->fade_triggered = true;
}

void snd_channel_set_arpeggio(uint8_t wait, uint8_t arp_vec_len, uint16_t arp_vec, struct snd_channel * pch) {
  pch->arp_wait = wait;
  pch->arp_timer = 0;
  pch->arp_vec_lenth = arp_vec_len;
  pch->arp_vec_data = arp_vec;
}

static void snd_channel__update_pitch_bend(struct snd_channel * pch);
static void snd_channel__update_fade_in_out(struct snd_channel * pch);
static void snd_channel__update_arpeggio(struct snd_channel * pch);

void snd_channel_update(struct snd_channel * pch) {
  if (0 <= pch->pitch) {
    // the channel is on
    snd_channel__update_pitch_bend(pch);
    snd_channel__update_fade_in_out(pch);
    snd_channel__update_arpeggio(pch);
  }
}

static void snd_channel__update_pitch_bend(struct snd_channel * pch) {
  if (!pch->pitch_delta) {
    return;
  }
  if (pch->pitch_timer) {
    pch->pitch_timer--;
    return;
  }
  pch->pitch_timer = pch->pitch_wait;
  pch->pitch += pch->pitch_delta;
  if (pch->pitch <= pch->pitch_min) {
    pch->pitch = pch->pitch_min;
    pch->pitch_min = PITCH_MIN;
    pch->pitch_delta = 0;

  }
  if (pch->pitch_max <= pch->pitch) {
    pch->pitch = pch->pitch_max;
    pch->pitch_max = PITCH_MAX;
    pch->pitch_delta = 0;
  }
}

static void snd_channel__update_fade_in_out(struct snd_channel * pch) {
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

static void snd_channel__update_arpeggio(struct snd_channel * pch) {
  if (!pch->arp_vec_data) {
    return;
  }
  if (pch->arp_timer) {
    pch->arp_timer--;
    return;
  }
  pch->arp_timer = pch->arp_wait;
  pch->arp = (pch->arp_vec_data >> 12) & 15;
  if (pch->arp_vec_lenth == 4) {
    pch->arp_vec_data &= 0x0fff;
    pch->arp_vec_data <<= 4;
    pch->arp_vec_data += pch->arp;
  }
  else if (pch->arp_vec_lenth == 3) {
    pch->arp_vec_data &= 0x0ff0;
    pch->arp_vec_data += pch->arp;
    pch->arp_vec_data <<= 4;
  }
}

static void snd_channel_set_noise(uint8_t ch, struct snd_channel * pch);
static void snd_channel_set_tone_flag(uint8_t ch, struct snd_channel * pch);
static void snd_channel_set_volume(uint8_t ch, struct snd_channel * pch);
static void snd_channel_set_modulation(uint8_t ch, struct snd_channel * pch);

void snd_channel_synthesis(struct snd_channel * pchs[3]) {
  PSG(7) = 0xb8;
  for (uint8_t ch = 3; ch--; ) {
    struct snd_channel * pch = pchs[ch];
    if (pch->pitch < 0 || 4 < pch->i.modulation) {
      PSG(ch+8) = 0;
    }
    else {
      // noise -------------------------------------------
      snd_channel_set_noise(ch, pch);
      // tone on/off -------------------------------------
      snd_channel_set_tone_flag(ch, pch);
      // volume or enable hardware envelope --------------
      snd_channel_set_volume(ch, pch);
      // modulation --------------------------------------
      snd_channel_set_modulation(ch, pch);
    }
  }
}

static void snd_channel_set_noise(uint8_t ch, struct snd_channel * pch) {
  if (pch->i.noise_fdr) {
    PSG(6) = pch->i.noise_fdr;
    PSG(7) &= ~(4 << ch);
  }
}
static void snd_channel_set_tone_flag(uint8_t ch, struct snd_channel * pch) {
  if (!pch->i.tone_on || pch->i.modulation == 1) {
    PSG(7) |= (1 << ch);
  }
}
static void snd_channel_set_volume(uint8_t ch, struct snd_channel * pch) {
  if (!pch->i.modulation) {
    int8_t volume = pch->volume + pch->i.volume - 15;
    if (volume < 0) {
      volume = 0;
    }
    else if (15 < volume) {
      volume = 15;
    }
    PSG(ch+8) = volume;
  }
  else {
    PSG(ch+8) = 16;
  }
}

static uint16_t snd_channel_calc_sw_period(uint16_t pitch, struct snd_channel * pch);
static uint16_t snd_channel_calc_hw_period(uint16_t pitch, struct snd_channel * pch);
inline void snd_channel_set_sw_period(uint8_t ch, uint16_t sw_period);
inline void snd_channel_set_hw_period(struct snd_channel * pch, uint16_t hw_period);

static void snd_channel_set_modulation(uint8_t ch, struct snd_channel * pch) {
  const int16_t pitch = pch->pitch + 256 * pch->arp + pch->p.pitch;
  uint16_t sw_period = pch->i.sw_period;
  uint16_t hw_period = pch->i.hw_period;
  // ---- SW only ------------------------------------
  if (pch->i.modulation == 0) {
    if (!sw_period) {
      sw_period = snd_channel_calc_sw_period(pitch, pch);
    }
  }
  // ---- HW only ------------------------------------
  else if (pch->i.modulation == 1) {
    if (!hw_period) {
      hw_period = snd_channel_calc_hw_period(pitch, pch);
    }
  }
  // ---- SW -> HW -----------------------------------
  else if (pch->i.modulation == 2) {
    if (!sw_period) {
      sw_period = snd_channel_calc_sw_period(pitch, pch);
    }
    if (!hw_period) {
      hw_period
        = (sw_period >> pch->i.ratio)
        + pch->i.hw_period_delta;
    }
  }
  // ----- HW -> SW -----------------------------------
  else if (pch->i.modulation == 3) {
    if (!hw_period) {
      hw_period = snd_channel_calc_hw_period(pitch, pch);
    }
    if (!sw_period) {
      sw_period
        = (hw_period << pch->i.ratio)
        + pch->i.sw_period_delta;
    }
  }
  // ----- SW + HW ------------------------------------
  else if (pch->i.modulation == 4) {
    if (!sw_period) {
      sw_period = snd_channel_calc_sw_period(pitch, pch);
    }
    if (!hw_period) {
      hw_period = snd_channel_calc_hw_period(pitch, pch);
    }
  }
  // ---- square wave ---------------------------------
  if (pch->i.modulation != 1) {
    snd_channel_set_sw_period(ch, sw_period);
  }
  // ---- saw / triangle wave -------------------------
  if (0 < pch->i.modulation) {
    snd_channel_set_hw_period(pch, hw_period);
  }
}

static uint16_t snd_channel_calc_sw_period(uint16_t pitch, struct snd_channel * pch) {
  return (snd__osc_period(pitch + pch->i.sw_pitch)
          + pch->i.sw_period_delta);
}
static uint16_t snd_channel_calc_hw_period(uint16_t pitch, struct snd_channel * pch) {
  return ((snd__osc_period(pitch + pch->i.hw_pitch) >> pch->i.ratio)
          + pch->i.hw_period_delta);
}
inline void snd_channel_set_sw_period(uint8_t ch, uint16_t sw_period) {
  PSG(2*ch+0) = (sw_period     ) & 0xff;
  PSG(2*ch+1) = (sw_period >> 8) & 0x0f;
}
inline void snd_channel_set_hw_period(struct snd_channel * pch, uint16_t hw_period) {
  PSG(11) = (hw_period     ) & 0xff;
  PSG(12) = (hw_period >> 8) & 0xff;
  if (pch->i.retrig) {
    // R13 = 8:Saw, 10:Triangle, 12:Inv-Saw, 14:Inv-Triangle
    PSG(13) = pch->i.waveform * 2 + 8;
  }
}
