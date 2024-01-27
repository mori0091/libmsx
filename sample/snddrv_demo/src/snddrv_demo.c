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
 * \file snddrv_demo.c
 */

#include <msx.h>
#include <ay_3_8910.h>
#include <screen.h>
#include <text.h>

// Include the header file "bgm.h" auto-generated from "bgm.aks".
#include "./bgm.h"

// Volume gauge --------------------------------------
// \see 'volume_gauge.c'
extern const char volume_gauge[17][5];
void init_volume_gauge_chars(void);

static void show_volume_levels(void) {
  for (uint8_t ch = 0; ch < 3; ++ch) {
    // uint8_t vol = psg_get(8+ch);
    uint8_t vol = ay_3_8910_buffer[8+ch];
    if (16 < vol) {
      vol = 16;
    }
    locate(10, 10+ch);
    print("VOL.#"); printc((char)'0'+ch); print(":"); print(volume_gauge[vol]);
  }
}

// Frequency -----------------------------------------
static void show_frequency(void) {
  locate( 0, 0); print("VSYNC:"); printi(msx_get_vsync_frequency());  print("Hz");
  locate( 0, 1); print(" PLAY:"); printi(snd_get_player_frequency()); print("Hz");
  locate( 0, 2); print("  BGM:"); printi(snd_get_bgm_frequency());    print("Hz(author expecting)");
}

// Main loop -----------------------------------------
_Noreturn
static void main_loop(void) {
  show_frequency();
  uint8_t old_pressed = 0;
  for (;;) {
    // await_vsync();
    show_volume_levels();

    // Is SPACE key pressed ?
    uint8_t pressed = joypad_get_state(0);
    uint8_t clicked = (old_pressed ^ pressed) & pressed;
    old_pressed = pressed;

    if (clicked & VK_FIRE_0) {
      // sound effects "coiiiiiiin!"
      snd_set_sfx(1, &bgm);
    }
    uint8_t Hz = snd_get_player_frequency();
    int8_t x = 0;
    if (Hz < 120 && (pressed & (VK_UP | VK_RIGHT))) {
      x = 1;
    }
    if (0 < Hz && (pressed & (VK_DOWN | VK_LEFT))) {
      x = -1;
    }
    if (x) {
      Hz += x;
      snd_set_player_frequency(Hz);
      locate(6, 1); printi(Hz); print("Hz ");
    }
  }
}

// Main (startup) ------------------------------------
void main(void) {
  // Turn key-click beep off.
  CLIKSW = 0;
  // Initiallize SCREEN mode
  screen1();
  locate(10,  7); print("- libmsx -");
  locate(10,  8); print("SNDDRV demo");
  locate( 1, 20); print("HIT SPACE TO PLAY SOUND EFFECT");
  locate( 3, 22); print("(Arrow key: speed up/down)");

  // Set character patterns for volume bar.
  init_volume_gauge_chars();

  // Initialize the sound driver.
  // This must be called once at least, before the snd_play() is called.
  snd_init();

  // Register the sound driver as VSYNC handler.
  set_vsync_handler(snd_play);

  snd_set_repeat(true);
  // snd_set_speed(SND_SPEED_1X * 1);  // 1x
  snd_set_bgm(0, &bgm);         // Register the BGM to be played.
  snd_start();                  // Start (or resume if paused) to play music.

  // snd_set_player_frequency(55); // Another method to control playing speed.

  main_loop();
}
