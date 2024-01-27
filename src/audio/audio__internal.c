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
 * \file audio__internal.c
 */

#include "audio__internal.h"

struct audio_Replayer audio_;

bool audio__null_decode_update(void) {
  return false;
}

void audio__null_decode_final(void) {
}

void audio__null_set_repeat(bool repeat) {
  (void)repeat;
}
