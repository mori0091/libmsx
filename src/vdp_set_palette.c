// -*- coding: utf-8-unix -*-

#include "../include/vdp.h"

#include "vdp_internal.h"

void vdp_set_palette(uint8_t idx, const palette_t palette) {
  uint8_t* p = (uint8_t*)&palette;
  __critical {
    VDP_SET_CTRL(16, idx);
    vdp_port2 = *p++;           /* r, b */
    vdp_port2 = *p++;           /* g */
  }
}
