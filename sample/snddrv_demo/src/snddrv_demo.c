// -*- coding: utf-8-unix -*-
/**
 * \file snddrv_demo.c
 *
 * Copyright (c) 2022 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */

#include <msx.h>
#include <snddrv.h>

#define ARRAY_SIZEOF(a)    (sizeof(a) / sizeof(a[0]))

// ---- Background music ----

const uint8_t bgm[] = {
                                                     // ch0     ch1      ch2
  0xe0, 0x9c,    0xe1, 0x9c,    0xe2, 0x9c,          // V12     V12      V12

  0x80, 0x3c,    0x81, 0x40,    0x82, 0x43,    0x14, // C4      E4       G4      T180 L4
  0x90,          0x91,          0x92,          0x14, // Off     Off      Off
  0x80, 0x3c,    0x81, 0x40,    0x82, 0x43,    0x14, // C4      E4       G4      T180 L4
  0x90,          0x91,          0x92,          0x14, // Off     Off      Off

  0x80, 0x3e,    0x81, 0x42,    0x82, 0x45,    0x14, // D4      F#4      A4      T180 L4
  0x90,          0x91,          0x92,          0x14, // Off     Off      Off
  0x80, 0x3e,    0x81, 0x42,    0x82, 0x45,    0x14, // D4      F#4      A4      T180 L4
  0x90,          0x91,          0x92,          0x14, // Off     Off      Off

  0x80, 0x3c,    0x81, 0x40,    0x82, 0x43,    0x14, // C4      E4       G4      T180 L4
  0x90,          0x91,          0x92,          0x14, // Off     Off      Off
  0x80, 0x3c,    0x81, 0x40,    0x82, 0x43,    0x14, // C4      E4       G4      T180 L4
  0x90,          0x91,          0x92,          0x14, // Off     Off      Off

  0x80, 0x37,    0x81, 0x3b,    0x82, 0x3e,          // G3      B3       D4      T180 L4
  0xe0, 0xb0, 4, 0xe1, 0xb0, 4, 0xe2, 0xb0, 4, 0x14, // fade out
  0xe0, 0xa0, 2, 0xe1, 0xa0, 2, 0xe2, 0xa0, 2, 0x04, // fade in
  0xe0, 0xb0, 2, 0xe1, 0xb0, 2, 0xe2, 0xb0, 2, 0x04, // fade out
  0xe0, 0xa0, 1, 0xe1, 0xa0, 1, 0xe2, 0xa0, 1, 0x02, // fade in
  0xe0, 0xb0, 1, 0xe1, 0xb0, 1, 0xe2, 0xb0, 1, 0x02, // fade out

                                               0x28, // R       R        R

  0xff,
};

_Noreturn
static void infinite_loop(void) {
  for (;;) {
    await_vsync();
  }
}

void main(void) {
  // Initialize the sound driver.
  // This must be called once at least, before the snd_play() is called.
  snd_init();

  // Register the sound driver as VSYNC handler.
  set_vsync_handler(snd_play);

  snd_set_repeat(true);
  snd_set_speed(SND_SPEED_1X * 2); // 2x
  snd_set_bgm((void *)bgm);     // Register the BGM to be played.
  snd_start();                  // Start the BGM.

  infinite_loop();
}
