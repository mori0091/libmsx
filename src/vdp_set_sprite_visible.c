// -*- coding: utf-8-unix -*-

#include "../include/vdp.h"

#include "vdp_internal.h"

void vdp_set_sprite_visible(bool visible) {
  __critical {
    if (visible) {
      RG8SAV &= ~0x02;
    } else {
      RG8SAV |= 0x02;
    }
    VDP_SET_CTRL(8, RG8SAV);
  }
}
