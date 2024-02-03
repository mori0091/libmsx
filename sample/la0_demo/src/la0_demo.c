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
 * \file la0_demo.c
 */

#include <msx.h>

#include <screen.h>
#include <text.h>

#include <audio.h>
#include <audio_buf.h>
#include <audio_efx_amp.h>
#include <audio_efx_psg2scc.h>

#include <la0.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// current song number
uint8_t song_number = 255;
uint8_t number_of_songs;
uint8_t number_of_sfx;
static MemFile bgm;
static MemFile sfx;

// joypad state
uint8_t pressed;
uint8_t clicked;

void show_vsync_freq(void) {
  locate(0, 0);
  printf("VSYNC:%dHz  ", (int)msx_get_vsync_frequency());
}

void show_player_freq(void) {
  locate(0, 1);
  printf(" PLAY:%dHz  ", (int)audio_get_bgm_frequency());
}

void show_bgm_info(void) {
  locate(0, 2);
  printf("  BGM:%dHz (#%d/%d)   ",
         (int)la0_get_bgm_frequency(),
         (int)song_number+1,
         (int)number_of_songs);
  show_player_freq();
  locate(26, 1); printf("%5zu", (size_t)la0_get_bgm_total_samples());
  locate(26, 2); printf("%5zu", (size_t)la0_get_bgm_loop_samples());
}

void increase_player_freq(void) {
  uint8_t Hz = audio_get_bgm_frequency();
  if (Hz < 240) {
    audio_set_bgm_frequency(Hz + 1);
    show_player_freq();
  }
}

void decrease_player_freq(void) {
  uint8_t Hz = audio_get_bgm_frequency();
  if (10 < Hz) {
    audio_set_bgm_frequency(Hz - 1);
    show_player_freq();
  }
}

void toggle_pause(void) {
  locate(13, 10);
  if (audio_is_paused()) {
    // resume
    audio_start();
    print("     ");
  }
  else {
    // pause
    audio_pause();
    print("PAUSE");
  }
}

void next_song(void) {
  if (song_number < number_of_songs - 1) {
    song_number++;
  }
  else {
    song_number = 0;
  }
  la0_set_bgm(song_number, &bgm);
  JIFFY = 0;
  show_bgm_info();
}

void prev_song(void) {
  if (0 < song_number) {
    song_number--;
  } else {
    song_number = number_of_songs - 1;
  }
  la0_set_bgm(song_number, &bgm);
  JIFFY = 0;
  show_bgm_info();
}

void show_psg_register_values(void) {
  for (uint8_t i = 0; i < 14; ++i) {
    locate(5, 4+i);
    uint8_t val = audio_buf_cache[0xb0+i];
    printx((uint16_t)(val >> 4));
    printx((uint16_t)(val & 15));
  }
}

void show_psg_registers(void) {
  locate(0, 4);
  for (uint8_t i = 0; i < 14; ++i) {
    printf("R#%02d\n", (int)i);
  }
  show_psg_register_values();
}

void update_joypad_state(void) {
  uint8_t joy = joypad_get_state(0);
  clicked = pressed & ~joy;
  pressed = joy;
}

static void play(void) {
  audio_play();                 // The libmsx audio replayer
  // audio_efx_psg2scc();          // Effector (PSG -> PSG, SCC/SCC+)
  audio_efx_amp();              // Effector (amp)
}

void main(void) {
  screen1();
  color(15, 4, 7);
  cls();

  number_of_songs = la0_open_resource(&bgm, "bgm.la0");
  number_of_sfx = la0_open_resource(&sfx, "sfx.la0");
  srand((unsigned int)time(NULL));

  locate(0, 21);
  print("  LEFT/RIGHT  Select BGM\n");
  print("   UP/DOWN    Player Freq.\n");
  if (number_of_sfx) {
    print("    SPACE     Sound effects\n");
  }
  else {
    print("    SPACE     Pause/Resume\n");
  }

  audio_init();
  set_vsync_handler(play);

  audio_set_repeat(true);

  await_vsync();
  show_vsync_freq();
  show_player_freq();
  show_psg_registers();

  for (;;) {
    await_vsync();

    locate(26, 0); printf("%5zu", (size_t)JIFFY);
    show_psg_register_values();

    if (!audio_is_playing_bgm()) {
      next_song();
    }

    // If the current song has a looped section, it repeats up to twice.
    if (2 <= la0_get_bgm_loop_counter()) {
      // Start fade-out, wait for it to complete, then stop the replayer.
      audio_efx_amp_fadeout(4);
      while (audio_efx_amp_get_fade()) {
        await_vsync();
      }
      audio_stop();
      // Reset the main volume, select the next song, and restart the replayer.
      audio_efx_amp_set_volume(15);
      next_song();
      audio_start();
    }

    update_joypad_state();
    if (clicked & VK_FIRE_0) {
      if (number_of_sfx) {
        la0_set_sfx(rand() % number_of_sfx, &sfx);
      }
      else {
        toggle_pause();         // pause / resume
      }
    }
    if (clicked & VK_RIGHT) {
      next_song();
    }
    if (clicked & VK_LEFT) {
      prev_song();
    }
    if (pressed & VK_UP) {
      increase_player_freq();
    }
    if (pressed & VK_DOWN) {
      decrease_player_freq();
    }
  }
}
