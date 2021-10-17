// -*- coding: utf-8-unix -*-

#include "../include/vdp.h"

#include "vdp_internal.h"

void vdp_write_control(uint8_t reg, void* src, uint8_t len) {
  __critical {
    VDP_SET_CTRL_PTR(reg);
    for (uint8_t* p = src; len--; ) {
      VDP_SET_CTRL_VALUE(*p++);
    }
  }
}
