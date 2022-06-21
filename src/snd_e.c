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
 * \file snd_e.c
 *
 */

#include "./snd_e.h"

void snd_e_note_on(struct snd_e_ctx * ctx) {
  // (re)start amplitude envelope
  (void)ctx;
}

void snd_e_note_off(struct snd_e_ctx * ctx) {
  // go to release phase
  (void)ctx;
}

void snd_e__program_change(struct snd_e_ctx * ctx, uint8_t index) {
  // set amplitude envelope table number
  (void)ctx;
  (void)index;
}

void snd_e__decode(struct snd_e_ctx * ctx) {
  (void)ctx;
}
