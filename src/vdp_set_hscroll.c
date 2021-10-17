// -*- coding: utf-8-unix -*-

#include "../include/vdp.h"

#include "vdp_internal.h"

void vdp_set_hscroll(uint16_t x) {
  uint8_t r26 = (x >> 3) & 0x3f;
  uint8_t r27 = (~x + 1) & 0x07;
  if (r27) r26++;
  __critical {
    RG26SA = r26;
    RG27SA = r27;
    VDP_SET_CTRL(26, r26);
    VDP_SET_CTRL(27, r27);
  }
}
