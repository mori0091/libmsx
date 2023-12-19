// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2023 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file audio__internal.h
 */

#ifndef AUDIO__INTERNAL_H_
#define AUDIO__INTERNAL_H_

#include <audio.h>
#include <audio_dec.h>

#include <scc.h>
#include <opll.h>

struct audio_Ctx {
  bool is_playing;              ///< Whether it is still playing music.
  uint8_t counter;              ///< Tick counter.
  uint8_t rate;                 ///< Replayer frequency.
  struct AudioDecoder decoder;
};

struct audio_Replayer {
  uint8_t vsync_freq;           ///< VSYNC frequency.
  struct audio_Ctx bgm;         ///< Replayer context for BGM.
  struct audio_Ctx sfx;         ///< Replayer context for SFX.
  bool paused;                  ///< Whether playback is paused or not.
  bool repeat;                  ///< Whether to loop playback.
  struct SCC scc;               ///< SCC/SCC+ handle.
  struct OPLL opll;             ///< OPLL handle.
};

extern struct audio_Replayer audio_;

extern bool audio__null_decode_update(void);
extern void audio__null_decode_final(void);
extern void audio__null_set_repeat(bool);

#endif // AUDIO__INTERNAL_H_
