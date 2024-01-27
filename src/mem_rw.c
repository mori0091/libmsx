// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2024 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file mem_rw.c
 */

#include "mem_rw.h"
#include "bios.h"
#include "workarea.h"

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint8_t * mem;

static void mem_set_write_address(uint8_t * dst) {
  mem = dst;
}

bool mem_is_opened(void) {
  return !!mem;
}

void mem_open(uint8_t * loc) {
  assert(!mem_is_opened());
  mem_set_write_address(loc);
}

void mem_close(void) {
  assert(mem_is_opened());
  mem_set_write_address(0);
}

uint8_t * mem_ptr(void) {
  return mem;
}

bool mem_is_in_free_area(uint8_t * p, size_t len) {
  return !((uintptr_t)p < (uintptr_t)PAGE_ADDR(3) ||
           (uintptr_t)p >= HIMEM                  ||
           (int16_t)(HIMEM - (uintptr_t)p) < len);
}

void mem_write_chunk(const uint8_t * src, size_t len) {
  assert(mem_is_opened());
  assert(len && mem_is_in_free_area(mem, len));
# if defined(__SDCC)
  memcpy(mem, src, len);
  mem += len;
# else
  while (len--) {
    *mem++ = *src++;
  }
# endif
}

void mem_read_chunk(uint8_t * dst, size_t len) {
  assert(mem_is_opened());
  assert(len && mem_is_in_free_area(dst, len));
# if defined(__SDCC)
  memcpy(dst, mem, len);
  mem += len;
# else
  while (len--) {
    *dst++ = *mem++;
  }
# endif
}

void mem_write_byte(uint8_t x) {
  assert(mem_is_opened());
  assert(mem_is_in_free_area(mem, 1));
  *mem++ = x;
}

uint8_t mem_read_byte(void) {
  assert(mem_is_opened());
  return *mem++;
}

void mem_dup(uint16_t offset, size_t n) {
  mem_write_chunk(mem - offset, n);
}
