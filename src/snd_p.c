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
 * \file snd_p.c
 *
 */

#include "./snd_p.h"

void snd_p_note_on(struct snd_p_ctx * ctx) {
  // (re)start pitch envelope
  (void)ctx;
}

void snd_p_note_off(struct snd_p_ctx * ctx) {
  // stop pitch envelope
  (void)ctx;
}

void snd_p__program_change(struct snd_p_ctx * ctx, uint8_t index) {
  // set pitch envelope table number
  (void)ctx;
  (void)index;
}

void snd_p__decode(struct snd_p_ctx * ctx) {
  (void)ctx;
}
