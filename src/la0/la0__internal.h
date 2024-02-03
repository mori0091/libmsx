// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2024 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file la0__internal.h
 */

#ifndef LA0__INTERNAL_H_
#define LA0__INTERNAL_H_

#include <la0.h>

#include "BitReader.h"

struct la0_song_state {
  struct BitReader in;          ///< Sequence of samples.
  MemFile tbl;                  ///< Concatnated dedicated samples.
};

struct la0_song_ctx {
  /** SCC/SCC+ Waveform table of the song. */
  MemFile wav;
  /** samples and the current state of its sequence. */
  struct la0_song_state state;
  /** for save/restore the start point of the loop part. */
  struct la0_song_state checkpoint;
};

struct la0_ctx {
  uint8_t rate;                 ///< default frequency.
  uint8_t soundchip_enable;     ///< bit#3: OPLL, bit#2: PSG, bit#1: SCC+, bit#0: SCC
  uint16_t total_samples;       ///< number of samples in total.
  uint16_t loop_samples;        ///< number of samples of the loop part.
  uint8_t scc_channel_mask;     ///< mask for SCC/SCC+ channel mask register.
  uint8_t psg_channel_mask;     ///< mask for PSG mixer register (R#7).
  uint16_t remaining_samples;   ///< remaining number of samples.
  uint8_t priority;             ///< priority (mainly used for SFX).
  bool repeat;                  ///< repeat loop part on/off
  uint8_t loop_counter;         ///< A counter that counts the number of loops.
                                ///< Starts at 0 and counts up when the song loops back.
  struct la0_song_ctx song;     ///< context for the song data.
};

extern struct la0 {
  struct la0_ctx bgm;
  struct la0_ctx sfx;
} la0_;

/**
 * Check LA0 file header, and count number of songs.
 *
 * \param mf  Opened memory-file pointer refering a LA0 file.
 * \return Number of songs contained in the LA0 file.
 */
int la0__count_songs(MemFile * mf);

/**
 * Check LA0 file header, and initialize LA0 context if given.
 *
 * \param idx Song number.
 * \param mf  Opened memory-file pointer refering a LA0 file.
 * \param ctx LA0 context to be initialized.
 * \return `true` if succeeded, `false` otherwise.
 */
bool la0__open(uint8_t idx, MemFile * mf, struct la0_ctx * ctx);

#endif // LA0__INTERNAL_H_
