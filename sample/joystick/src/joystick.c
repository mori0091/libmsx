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
 * \file joystick.c
 * \brief Sample : get and show joystick button state.
 */

#include <msx.h>
#include <screen.h>
#include <text.h>

static void puts_joystate(const uint8_t state, const char* str) {
  for (int i = 7; i >= 0; --i) {
    printc('0' + ((state >> i) & 1));
  }
  print(str);
}

static void print_joypad_state(void) {
  locate(0,0);
  puts_joystate(joypad_get_state(0), " : SPACE and ARROW keys\n");
  puts_joystate(joypad_get_state(1), " : joystick #1 (port A)\n");
  puts_joystate(joypad_get_state(2), " : joystick #2 (port B)\n");
  print("  ||||||\n"
        "  |||||`-- VK_UP\n"
        "  ||||`--- VK_DOWN\n"
        "  |||`---- VK_LEFT\n"
        "  ||`----- VK_RIGHT\n"
        "  |`------ VK_FIRE_0\n"
        "  `------- VK_FIRE_1\n");
}

void main(void) {
  screen1();
  color(15, 4, 7);

  for (;;) {
    await_vsync();
    print_joypad_state();
  }
}
