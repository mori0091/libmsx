// -*- coding: utf-8-unix -*-

#include "../include/vdp.h"

#include "vdp_internal.h"

uint8_t vdp_get_status(uint8_t reg) {
  volatile uint8_t x;
  __critical {
    VDP_SET_CTRL(15, reg);
    x = vdp_port1;
    VDP_SET_CTRL(15, 0);
  }
  return x;
}
