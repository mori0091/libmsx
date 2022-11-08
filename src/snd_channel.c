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
#include "./snd_osc_period.h"

static void snd_channel_reset_arpeggio(struct snd_channel * pch);
static void snd_channel_reset_period_bend(struct snd_channel * pch);
static void snd_channel_reset_pitch_bend(struct snd_channel * pch);
static void snd_channel_reset_pitch_glide(struct snd_channel * pch);
static void snd_channel_reset_fade(struct snd_channel * pch);

void snd_channel_note_on(uint8_t note, const snd_Instrument * inst, struct snd_channel * pch) {
  pch->period_offset = 0;
  {
    uint8_t flag = pch->flag;
    pch->flag = 0;
    if (!(flag & FADE_TRIG)) {
      // Turn fade-in/out off
      snd_channel_reset_fade(pch);
    }
    if (!(flag & PERIOD_TRIG)) {
      // Turn period-bend off
      snd_channel_reset_period_bend(pch);
    }
    if (!(flag & PITCH_TRIG)) {
      // Turn pitch-bend off
      snd_channel_reset_pitch_bend(pch);
    }
  }
  // --------------------------------------------------------
  const uint16_t pitch = (note << 8) + pch->t.detune;
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
    snd_channel_reset_pitch_glide(pch);
  }
  else {
    // ---- Legato : switch to the given note w/o attack
    pch->pitch = pitch;
    // ---- Normal NoteOn (attack)
    if (inst) {
      snd_i__program_change(&pch->i, inst);
      snd_i_note_on(&pch->i);
      snd_a_note_on(&pch->a);
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

void snd_channel_reset_expression(struct snd_channel * pch) {
  snd_a__program_change(&pch->a, NULL); // arpeggio off
  snd_p__program_change(&pch->p, NULL); // pitch envelope off
  snd_channel_reset_arpeggio(pch);
  snd_channel_reset_period_bend(pch);
  snd_channel_reset_pitch_bend(pch);
  snd_channel_reset_pitch_glide(pch);
  snd_channel_reset_fade(pch);
}

static void snd_channel_reset_arpeggio(struct snd_channel * pch) {
  pch->arp = 0;
  pch->arp_vec_data = 0;
}
static void snd_channel_reset_period_bend(struct snd_channel * pch) {
  pch->period_delta = 0;
  pch->period_offset = 0;
  // pch->pitch_min = PITCH_MIN;
  // pch->pitch_max = PITCH_MAX;
}
static void snd_channel_reset_pitch_bend(struct snd_channel * pch) {
  pch->pitch_delta = 0;
}
static void snd_channel_reset_pitch_glide(struct snd_channel * pch) {
  pch->pitch_glide = 0;
}
static void snd_channel_reset_fade(struct snd_channel * pch) {
  pch->fade_speed = 0;
}

void snd_channel_set_period_bend(int8_t sign, uint16_t period_delta, struct snd_channel * pch) {
  pch->period_delta = period_delta;
  pch->period_timer = 0;
  pch->period_sign = sign;
  pch->flag |= PERIOD_TRIG;
}

void snd_channel_set_pitch_bend(uint8_t wait, int16_t pitch_delta, struct snd_channel * pch) {
  pch->pitch_wait = wait;
  pch->pitch_timer = 0;
  pch->pitch_delta = pitch_delta;
  pch->flag |= PITCH_TRIG;
  pch->pitch_min = PITCH_MIN;
  pch->pitch_max = PITCH_MAX;
}

void snd_channel_set_fade(int8_t fade, uint16_t speed, struct snd_channel * pch) {
  pch->fade_speed = speed;
  pch->fade_timer = 0;
  pch->fade = fade;
  pch->flag |= FADE_TRIG;
}

void snd_channel_set_arpeggio(uint8_t wait, uint8_t arp_vec_len, uint16_t arp_vec, struct snd_channel * pch) {
  pch->arp_wait = wait;
  pch->arp_timer = 0;
  pch->arp_vec_length = arp_vec_len;
  pch->arp_vec_data = arp_vec;
}

static void snd_channel__update_period_bend(struct snd_channel * pch);
static void snd_channel__update_pitch_bend(struct snd_channel * pch);
static void snd_channel__update_fade_in_out(struct snd_channel * pch);
static void snd_channel__update_arpeggio(struct snd_channel * pch);

void snd_channel_update(struct snd_channel * pch) {
  if (0 <= pch->pitch) {
    // the channel is on
    snd_channel__update_period_bend(pch);
    snd_channel__update_pitch_bend(pch);
    snd_channel__update_fade_in_out(pch);
    snd_channel__update_arpeggio(pch);
  }
}

static void snd_channel__update_period_bend(struct snd_channel * pch) {
  if (!pch->period_delta) {
    return;
  }
  const uint16_t acc = pch->period_timer + pch->period_delta;
  const uint16_t delta = acc >> 4;
  pch->period_timer &= 15;
  if (0 <= pch->period_sign) {
    pch->period_offset += delta;
    if (4095 < pch->period_offset) {
      pch->period_offset = 4095;
    }
  }
  else {
    pch->period_offset -= delta;
    if (pch->period_offset < -4095) {
      pch->period_offset = -4095;
    }
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
  if (!pch->fade_speed) {
    return;
  }
  const uint16_t acc = pch->fade_timer + pch->fade_speed;
  const uint8_t delta = acc >> 7;
  pch->fade_timer = acc & 0x7f;
  // fade in
  if (0 <= pch->fade) {
    pch->volume += delta;
    if (15 < pch->volume) {
      pch->volume = 15;
    }
  }
  // fade out
  else {
    if (pch->volume <= delta) {
      pch->volume = 0;
    }
    else {
      pch->volume -= delta;
    }
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
  if (pch->arp_vec_length == 4) {
    // pch->arp_vec_data &= 0x0fff;
    // pch->arp_vec_data <<= 4;
    // pch->arp_vec_data += pch->arp;
    pch->arp_vec_data = (pch->arp_vec_data << 4) + pch->arp;
  }
  else if (pch->arp_vec_length == 3) {
    // pch->arp_vec_data &= 0x0ff0;
    // pch->arp_vec_data += pch->arp;
    // pch->arp_vec_data <<= 4;
    pch->arp_vec_data = (pch->arp_vec_data + pch->arp) << 4;
  }
}
