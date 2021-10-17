// -*- coding: utf-8-unix -*-

#include "../include/vdp.h"

#include "vdp_internal.h"

void vdp_set_visible(bool visible) {
  __critical {
    if (visible) {
      RG1SAV |= 0x40;
    } else {
      RG1SAV &= ~0x40;
    }
    VDP_SET_CTRL(1, RG1SAV);
  }
}
