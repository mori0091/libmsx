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

#include "./screen1.h"

// User-defined instruments / timbres. ---------------
// \see `instruments.c`
extern const size_t i_tables_length;
extern const struct snd_i_table * i_tables[];

// Sound effect "coin" -------------------------------
// \see 'sfx_coin.c'
extern const uint8_t sfx_coin[];

// Background music ----------------------------------
// \see 'bgm_01.c'
extern const uint8_t bgm_01[];

// Volume gauge --------------------------------------
// \see 'volume_gauge.c'
extern const char volume_gauge[17][5];
void init_volume_gauge_chars(void);

static void show_volume_levels(void) {
  for (uint8_t ch = 0; ch < 3; ++ch) {
    uint8_t vol = psg_get(8+ch);
    locate(10, 10+ch);
    puts("VOL.#"); putc('0'+ch); puts(":"); puts(volume_gauge[vol]);
  }
}

// Frequency -----------------------------------------
static void show_frequency(void) {
  locate( 0, 0); puts("VSYNC:"); puti(msx_get_vsync_frequency());  puts("Hz");
  locate( 0, 1); puts(" PLAY:"); puti(snd_get_player_frequency()); puts("Hz");
  locate( 0, 2); puts("  BGM:"); puti(snd_get_bgm_frequency());    puts("Hz(composer expecting)");
}

// Main loop -----------------------------------------
_Noreturn
static void main_loop(void) {
  show_frequency();
  bool old_fired = false;
  for (;;) {
    await_vsync();
    show_volume_levels();
    // Is SPACE key pressed ?
    bool fired = joypad_get_state(0) & VK_FIRE_0;
    if (!old_fired && fired) {
      // sound effects "coiiiiiiin!"
      snd_set_sfx((void *)sfx_coin);
    }
    old_fired = fired;
  }
}

// Main (startup) ------------------------------------
void main(void) {
  // Turn key-click beep off.
  CLIKSW = 0;
  // Initiallize SCREEN mode
  screen1();
  locate(10,  8); puts("SNDDRV demo");
  locate( 1, 20); puts("HIT SPACE TO PLAY SOUND EFFECT");

  // Set character patterns for volume bar.
  init_volume_gauge_chars();

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

  // snd_set_player_frequency(55); // Another method to control playing speed.

  main_loop();
}
