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
 * \file audio_set_repeat.c
 */

#include "audio__internal.h"

void audio_set_repeat(bool repeat) {
  __asm__("di");
  audio_.repeat = repeat;
  if (audio_.bgm.decoder.set_repeat) {
    audio_.bgm.decoder.set_repeat(repeat);
  }
  __asm__("ei");
}
