// -*- coding: utf-8-unix -*-

#include "../include/io.h"
#include "../include/input.h"

#define KANA_LAMP_OFF  (0x080)

uint8_t joypad_get_state(uint8_t controller) {
  volatile uint8_t r14 = 0;
  volatile uint8_t r15 = 0;
  switch (controller) {
  case 0:
    // TODO keypad
    return 0x00;
  case 1:
    // joystick #1
    r15 = 0x13;
    break;
  case 2:
    // joystick #1
    r15 = 0x6c;
    break;
  default:
    // error
    return 0xff;
  }
  for (;;) {
    volatile uint8_t x;
    __critical {
      psg_port0 = 15;
      psg_port1 = r15 | KANA_LAMP_OFF;
      psg_port0 = 14;
      x = psg_port2;
      // bit #3..#0 of PSG port B (PSG #15) shall be H level
      // during the PSG port B is not used for output.
      // (see MSX Datapack Volume 1, pp.44)
      psg_port0 = 15;
      psg_port1 = 0x0f | KANA_LAMP_OFF;
    }
    if (r14 == x) break;
    r14 = x;
  }
  return (~r14 & 0x3f);
}
