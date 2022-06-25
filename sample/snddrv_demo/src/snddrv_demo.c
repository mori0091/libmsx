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
 * \file snddrv_demo.c
 */

#include <msx.h>
#include <snddrv.h>
#include <snd_i_table.h>

// User-defined instruments / timbres.
// \see `instruments.c`
extern const size_t i_tables_length;
extern const struct snd_i_table * i_tables[];

// Sound effect "coin"
// \see 'sfx_coin.c'
extern const uint8_t sfx_coin[];

// Background music
// \see 'bgm_01.c'
extern const uint8_t bgm_01[];

_Noreturn
static void infinite_loop(void) {
  bool old_fired = false;
  for (;;) {
    // await_vsync();
    // Is SPACE key pressed ?
    bool fired = joypad_get_state(0) & VK_FIRE_0;
    if (!old_fired && fired) {
      // sound effects "coiiiiiiin!"
      snd_set_sfx((void *)sfx_coin);
    }
    old_fired = fired;
  }
}

void main(void) {
  // Turn key-click beep off.
  CLIKSW = 0;
  // Initialize the sound driver.
  // This must be called once at least, before the snd_play() is called.
  snd_init();

  // Register instruments (timbre) table.
  snd_set_i_tables(i_tables_length, i_tables);

  // Register the sound driver as VSYNC handler.
  set_vsync_handler(snd_play);

  snd_set_repeat(true);
  snd_set_speed(SND_SPEED_1X * 1); // 1x
  snd_set_bgm((void *)bgm_01);     // Register the BGM to be played.
  snd_start();                     // Start (or resume if paused) to play music.

  infinite_loop();
}
