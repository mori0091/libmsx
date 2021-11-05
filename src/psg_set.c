// -*- coding: utf-8-unix -*-

#include "../include/io.h"
#include "../include/psg.h"

void psg_set(uint8_t reg, uint8_t val) {
  __critical {
    psg_port0 = reg;
    psg_port1 = val;
  }
}
