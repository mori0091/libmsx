// -*- coding: utf-8-unix -*-

#include "../include/vdp.h"

#include "vdp_internal.h"

void vdp_set_hscroll_mask(bool enable) {
  __critical {
    if (enable) {
      RG25SA |= 0x02;
    } else {
      RG25SA &= ~0x02;
    }
    VDP_SET_CTRL(25, RG25SA);
  }
}
