// -*- coding: utf-8-unix -*-

#include <msx.h>

#include "sound.h"

const uint8_t channel_1[] ={
  0x60, 0x1e, 0x88, 0x01, 0xac, // T120L4 V8 C
  0x40, 0x1e, 0x01, 0x7d,       // T120L4    D
  0x40, 0x1e, 0x01, 0x53,       // T120L4    E
  0x20, 0x1e, 0x80,             // T120L4    R
  0xff,                         // end of stream
};
const uint8_t channel_2[] = {
  0x60, 0x1e, 0x88, 0x00, 0xd6, // T120L4 V8 O5C
  0x40, 0x1e, 0x00, 0xbe,       // T120L4    O5D
  0x40, 0x1e, 0x00, 0xaa,       // T120L4    O5E
  0x20, 0x1e, 0x80,             // T120L4 R
  0xff,                         // end of stream
};
const uint8_t channel_3[] = {
  0x60, 0x1e, 0x88, 0x02, 0x3b, // T120L4 V8 O3G
  0x40, 0x1e, 0x01, 0xfd,       // T120L4    O3A
  0x40, 0x1e, 0x01, 0xc5,       // T120L4    O3B
  0x20, 0x1e, 0x80,             // T120L4 R
  0xff,                         // end of stream
};

const struct sound_clip bgm = {
  .streams = {
    [0] = channel_1,
    [1] = channel_2,
    [2] = channel_3,
  }
};

_Noreturn
static void infinite_loop(void) {
  for (;;) {
    await_vsync();
  }
}

void main(void) {
  set_vsync_handler(sound_player);
  sound_set_repeat(true);
  sound_start(&bgm);
  infinite_loop();
}
