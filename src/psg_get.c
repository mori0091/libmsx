// -*- coding: utf-8-unix -*-

#include "../include/io.h"
#include "../include/psg.h"

uint8_t psg_get(uint8_t reg) {
  uint8_t val;
  __critical {
    psg_port0 = reg;
    val = psg_port2;
  }
  return val;
}
