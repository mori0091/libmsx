// -*- coding: utf-8-unix -*-

#include "../include/vdp.h"

#include "vdp_internal.h"

void vdp_write_palette(const palette_t palettes[16]) {
  uint8_t* p = (uint8_t*)palettes;
  __critical {
    VDP_SET_CTRL(16, 0);
    for (uint8_t i = 16; i--; ) {
      vdp_port2 = *p++;  /* R, B */
      vdp_port2 = *p++;  /* G */
    }
  }
}
