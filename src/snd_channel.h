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
 * \file snd_channel.h
 *
 */

#ifndef SND_CHANNEL_H_
#define SND_CHANNEL_H_

#include "./snd_i.h"
#include "./snd_a.h"
#include "./snd_p.h"

struct snd_channel {
  struct snd_i_ctx i;           // context for instrument
  struct snd_a_ctx a;           // context for arpeggio
  struct snd_p_ctx p;           // context for pitch envelope
  // ----
  struct {
    int16_t pitch;              // (note number) << 8 ; i.e. [1/256 note]
    uint8_t volume;             // current volume level
    int8_t  arp;                // +/- note number
  };
  // ---- expression (fade-in, fade-out) ----
  struct {
    uint16_t fade_wait;
    uint16_t fade_timer;
    int8_t   fade;              // off (0), fade-in (+1), fade-out (-1)
    bool     fade_triggered;
  };
  // ---- expression (pitch bend) ----
  struct {
    uint16_t pitch_wait;
    uint16_t pitch_timer;
    int16_t  pitch_delta;       // off (0), +/- pitch
    bool     pitch_triggered;
  };
  struct {
    uint16_t pitch_glide;       // off (0), pitch
    uint16_t pitch_min;         // PITCH_MIN or glide target (be set on NoteOn message)
    uint16_t pitch_max;         // PITCH_MAX or glide target (be set on NoteOn message)
  };
};

/**
 * Synthesis the given 3 channels' state and set to PSG register buffer.
 *
 * \param pchs    pointers to 3 channels.
 */
void snd_channel_synthesis(struct snd_channel * pchs[3]);

// ------------------------------------------------------------------------------------

/**
 * Update fade-in/out and pitch-bend state of a channel
 *
 * \param pch   pointer to a channel
 */
void snd_channel_update(struct snd_channel * pch);

void snd_channel_note_on(uint8_t note, struct snd_channel * pch);

void snd_channel_note_off(struct snd_channel * pch);

void snd_channel_reset_expression(struct snd_channel * pch);

/**
 * Add some value to the current pitch of a channel.
 *
 * \param pch          pointer to a channel
 * \param pitch_delta  value of pitch bend
 * \return `true` if succeeded, `false` if failed or the pitch reached to
 *          upper/lower limit.
 */
bool snd_channel_add_pitch(struct snd_channel * pch, int16_t pitch_delta);

void snd_channel_set_pitch_bend(uint8_t wait, int16_t pitch_delta, struct snd_channel * pch);

void snd_channel_set_fade(int8_t fade, uint16_t wait, struct snd_channel * pch);

#endif // SND_CHANNEL_H_
