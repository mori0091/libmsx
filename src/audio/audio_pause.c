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
 * \file audio_pause.c
 */

#include "audio__internal.h"

#include <audio_buf.h>

void audio_pause(void) {
  __asm__("di");
  audio_.paused = true;
  audio_buf_stop();
  __asm__("ei");
}
