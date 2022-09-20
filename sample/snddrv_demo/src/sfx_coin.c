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
 * \file sfx_coin.c
 * A music data example of snd_Stream type for snddrv.
 */

#include <snd_sound.h>

static const uint8_t stream[] = {
  //ch, expressions...,  note, i#, wait, // channel: note i# expressions         wait+1
  0x81, 0x90, 0x8f,        83,  3, 0x02, // PSG chB:  B_5 03 vF.. .... .... .... 03
  0x81,                    88,  3, 0x1f, // PSG chB:  B_6 03 .... .... .... .... 20
  0xff,                                  // EOM
};

const snd_Stream sfx_coin = {
  .replayRate = 60,             // 60Hz
  .stream = stream,
};
