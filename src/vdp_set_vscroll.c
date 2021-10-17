// -*- coding: utf-8-unix -*-

#include "../include/vdp.h"

#include "vdp_internal.h"

void vdp_set_vscroll(uint8_t y) {
  __critical {
    RG23SA = y;
    VDP_SET_CTRL(23, y);
  }
}
