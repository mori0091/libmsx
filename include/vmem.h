// -*- coding: utf-8-unix -*-

#ifndef VMEM_H
#define VMEM_H

#pragma once

#include <stdint.h>

#include "io.h"

// ---- VRAM access

typedef uint32_t vmemptr_t;

void vmem_set_read_address(vmemptr_t loc);

void vmem_set_write_address(vmemptr_t loc);

inline uint8_t vmem_get(void) {
  return vdp_port0;
}

inline void vmem_set(uint8_t val) {
  vdp_port0 = val;
}

void vmem_read(vmemptr_t src, void* dst, uint16_t len);

void vmem_write(vmemptr_t dst, void* src, uint16_t len);

void vmem_memset(vmemptr_t dst, uint8_t val, uint16_t len);

#endif
