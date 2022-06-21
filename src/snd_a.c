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
 * \file snd_a.c
 *
 */

#include "./snd_a.h"

void snd_a_note_on(struct snd_a_ctx * ctx) {
  // (re)start instrument
  (void)ctx;
}

void snd_a_note_off(struct snd_a_ctx * ctx) {
  // stop arpeggio
  (void)ctx;
}

void snd_a__program_change(struct snd_a_ctx * ctx, uint8_t index) {
  // set arpeggio table number
  (void)ctx;
  (void)index;
}

void snd_a__decode(struct snd_a_ctx * ctx) {
  (void)ctx;
}
