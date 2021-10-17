// -*- coding: utf-8-unix -*-

#include "../include/vdp.h"

#include "vdp_internal.h"

void vmem_memset(vmemptr_t dst, uint8_t val, uint32_t len) {
  __critical {
    vmem_set_write_address(dst);
    for (; len--; ) {
      vdp_port0 = val;
    }
  }
}
