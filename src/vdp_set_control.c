// -*- coding: utf-8-unix -*-

#include "../include/vdp.h"

#include "vdp_internal.h"

void vdp_set_control(uint8_t reg, uint8_t x) {
  __critical {
    VDP_SET_CTRL(reg, x);
  }
}
