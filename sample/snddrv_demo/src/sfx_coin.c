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
 */

#include <snd_sound.h>

// ---- Sound effects ----
static const uint8_t stream[] = {
//ch0            ch1            ch3            wait  // ch0     ch1      ch2
                 0xc1, 2,                            //         i#2
                 0xe1, 0x8f,                         //         V15
                 0x81, 83,                     0x02, //         B5               T225 L32.
                 0x81, 88,                     0x1f, //         E6               T225 L2

  0xff,
};

const snd_Sound sfx_coin = {
  .tag = SND_STREAM,
  .replayRate = 60,             // 60Hz
  .stream.data = stream,
};
