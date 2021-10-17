// -*- coding: utf-8-unix -*-

#include "../include/vdp.h"

#include "vdp_internal.h"

void vdp_set_sprite_size(enum sprite_size size) {
  __critical {
    RG1SAV = (RG1SAV & ~0x03) | size;
    VDP_SET_CTRL(1, RG1SAV);
  }
}
