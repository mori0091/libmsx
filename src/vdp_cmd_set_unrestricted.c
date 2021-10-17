// -*- coding: utf-8-unix -*-

#include "../include/vdp.h"

#include "vdp_internal.h"

void vdp_cmd_set_unrestricted(bool enable) {
  __critical {
    if (enable) {
      RG25SA |= 0x40;
    } else {
      RG25SA &= ~0x40;
    }
    VDP_SET_CTRL(25, RG25SA);
  }
}
