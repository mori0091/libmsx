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
 * \file snd_channel.h
 *
 */

#ifndef SND_CHANNEL_H_
#define SND_CHANNEL_H_

#include "./snd_t.h"
#include "./snd_i.h"
#include "./snd_a.h"
#include "./snd_p.h"
#include <stdint.h>

#define FADE_TRIG    (1 << 0)
#define PERIOD_TRIG  (1 << 1)
#define PITCH_TRIG   (1 << 2)

struct snd_channel {
  struct snd_t_ctx t;           // context for track
  struct snd_i_ctx i;           // context for instrument
  struct snd_p_ctx a;           // context for arpeggio
  struct snd_p_ctx p;           // context for pitch envelope
  // ----
  struct {
    int16_t pitch;              // (note number) << 8 ; i.e. [1/256 note]
    int16_t period_offset;      // +/- period (wave length)
    uint8_t volume;             // current volume level
    int8_t  arp;                // +/- note number
  };
  // ----
  uint8_t flag;
  // ---- expression (fade-in, fade-out) ----
  struct {
    uint16_t fade_speed;        // 1 lv / 128 ticks
    uint16_t fade_timer;
    int8_t   fade;              // off (0), fade-in (+1), fade-out (-1)
  };
  // ---- expression (pitch bend) ----
  struct {
    uint16_t period_delta;      // off (0)
    uint16_t period_timer;
    int8_t   period_sign;
  };
  struct {
    uint16_t pitch_wait;
    uint16_t pitch_timer;
    int16_t  pitch_delta;       // off (0), +/- pitch
  };
  struct {
    uint16_t pitch_glide;       // off (0), pitch
    uint16_t pitch_min;         // PITCH_MIN or glide target (be set on NoteOn message)
    uint16_t pitch_max;         // PITCH_MAX or glide target (be set on NoteOn message)
  };
  // ---- expression (arpeggio) ----
  struct {
    uint16_t arp_wait;
    uint16_t arp_timer;
    uint16_t arp_vec_data;      // vector of 4bit values (0x0000: off)
    uint8_t  arp_vec_length;    // 3: 3 notes, 4: 4 notes
  };
};

/**
 * Synthesis the given 3 channels' state and set to PSG register buffer.
 *
 * \param pchs    pointers to 3 channels.
 */
void snd_channel_synthesis(uint8_t ch, struct snd_channel * pch);

// ------------------------------------------------------------------------------------

/**
 * Update the state of ongoing expression commands for a channel.
 * - arpeggio,
 * - pitch/period-slide, pitch-glide,
 * - fade-in/out
 *
 * \param pch   pointer to a channel
 */
void snd_channel_update(struct snd_channel * pch);

void snd_channel_note_on(uint8_t note, const snd_Instrument * inst, struct snd_channel * pch);

void snd_channel_note_off(struct snd_channel * pch);

void snd_channel_reset_expression(struct snd_channel * pch);

void snd_channel_set_period_bend(int8_t sign, uint16_t period_delta, struct snd_channel * pch);

void snd_channel_set_pitch_bend(uint8_t wait, int16_t pitch_delta, struct snd_channel * pch);

void snd_channel_set_fade(int8_t fade, uint16_t wait, struct snd_channel * pch);

void snd_channel_set_arpeggio(uint8_t wait, uint8_t arp_vec_len, uint16_t arp_vec, struct snd_channel * pch);

#endif // SND_CHANNEL_H_
