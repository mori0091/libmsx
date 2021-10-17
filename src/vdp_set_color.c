// -*- coding: utf-8-unix -*-

#include "../include/vdp.h"

#include "vdp_internal.h"

void vdp_set_color(uint8_t c) {
  __critical {
    RG7SAV = c;
    VDP_SET_CTRL(7, c);
  }
}
