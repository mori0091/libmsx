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
 * \file bmem_rd.c
 */

#include "bmem_rd.h"

#include "bios.h"

#include <assert.h>
#include <stdint.h>
#include <string.h>

struct bmem_rd {
  uint8_t old_bank;
  uint8_t bank;
  const uint8_t * loc;
};

static struct bmem_rd bmem;

#define bmem_offset_of(loc) (uint16_t)((loc) & (PAGE_SIZE - 1))

bool bmem_is_opened(void) {
  return !!bmem.old_bank;
}

void bmem_open(bmemptr_t loc) {
  assert(!bmem_is_opened());
  bmem.old_bank = bmem_get_bank();
  bmem.loc = (const uint8_t *)PAGE_ADDR(2) + bmem_offset_of(loc);
  bmem.bank = bmem_bank_of(loc);
  bmem_set_bank(bmem.bank);
}

void bmem_close(void) {
  assert(bmem_is_opened());
  bmem_set_bank(bmem.old_bank);
  bmem.old_bank = 0;
}

size_t bmem_avail(void) {
  return bmem_is_opened()
    ? (const uint8_t *)PAGE_ADDR(3) - bmem.loc
    : 0;
}

const uint8_t * bmem_ptr(void) {
  return bmem_is_opened() ? bmem.loc : 0;
}

void bmem_ensure(void) {
  assert(bmem_is_opened());
  if ((const uint8_t *)PAGE_ADDR(3) <= bmem.loc) {
    bmem.loc -= PAGE_SIZE;
    bmem_set_bank(++bmem.bank);
  }
}

void bmem_skip(size_t len) {
  assert(bmem_is_opened());
  bmem.loc += len;
  bmem_ensure();
}

void bmem_next_segment(void) {
  assert(bmem_is_opened());
  bmem.loc = PAGE_ADDR(2);
  bmem_set_bank(++bmem.bank);
}

void bmem_read_chunk(uint8_t * dst, size_t len) {
  assert(bmem_is_opened());
  bmem_ensure();
  size_t n = bmem_avail();
  while (n <= len) {
    memcpy(dst, bmem.loc, n);
    dst += n;
    len -= n;
    n = PAGE_SIZE;
    bmem_next_segment();
  }
  if (len) {
    memcpy(dst, bmem.loc, len);
    bmem_skip(len);
  }
}

uint8_t bmem_read_byte(void) {
  assert(bmem_is_opened());
  bmem_ensure();
  return *bmem.loc++;
}
