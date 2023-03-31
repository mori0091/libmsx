// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2023 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file ZX0_decompress_bmem_to_vmem_buffer.c
 */

#include "ZX0_decompress.h"

#include "./ZX0__decode.h"
#include "bios.h"
#include "workarea.h"
#include "xmem.h"

#include <assert.h>
#include <string.h>

#define BUF_SIZE  (size_t)(PAGE_SIZE)
#define BUF_ADDR  ((uint8_t *)PAGE_ADDR(0))

static size_t rem;
static uint8_t * ptr;

uint8_t main_ram_slot;
volatile uint8_t * ex_slot_sel_p; // pointer to SLTTBL[i]
uint8_t ex_slot_sel;              // saved value of SLTTBL[i]

static void reset(void) {
  rem = BUF_SIZE;
  ptr = BUF_ADDR;
}

static void init(void) {
  reset();
  // \note assuming that the slot # of main RAM page #0 is same as page #3.
  main_ram_slot = msx_get_slot(PAGE_ADDR(3));
  ex_slot_sel_p = &SLTTBL[main_ram_slot & 3];
}

// -------------------------------------------------------
// \note
// The BIOS function `ENASLT` cannot be used to switch page #0 between main ROM
// and main RAM. `ENASLT` probably be used to switch from ROM to RAM, but not
// from RAM to ROM, because the BIOS function resides in main ROM.

/** `MSX` Secondary slot select register. */
static volatile __at (0xffff) uint8_t secondary_slot_select_register;

static void expose_cache(void) {
  __asm__("di");

  // set primary slot # of main RAM on page #0
  __asm__("ld  a, (_main_ram_slot)");
  __asm__("and #0x03");
  __asm__("ld  b, a");
  __asm__("in  a, (_primary_slot_select_port)");
  __asm__("and #0xfc");
  __asm__("or  b");
  __asm__("out (_primary_slot_select_port), a");

  // if (main_ram_slot & 0x80)
  {
    // \note assuming that the slot # of main RAM page #0 is same as page #3.

    // save current secondary slot #s of main RAM primary slot.
    uint8_t x = *ex_slot_sel_p;
    ex_slot_sel = x;
    // set secondary slot # of main RAM.
    secondary_slot_select_register
      = *ex_slot_sel_p
      = (x & 0xfc) | ((main_ram_slot >> 2) & 3);
  }
}

static void hide_cache(void) {
  // if (main_ram_slot & 0x80)
  {
    // \note assuming that the slot # of main RAM page #0 is same as page #3.

    // restore secondary slot #s of main RAM primary slot.
    secondary_slot_select_register
      = *ex_slot_sel_p
      = ex_slot_sel;
  }

  // set primary slot # of main ROM slot
  __asm__("ld  a, (_EXPTBL+0)");
  __asm__("and #0x03");
  __asm__("ld  b, a");
  __asm__("in  a, (_primary_slot_select_port)");
  __asm__("and #0xfc");
  __asm__("or  b");
  __asm__("out (_primary_slot_select_port), a");

  __asm__("ei");
}

// -------------------------------------------------------
static void copy_src_to_cache(size_t len) {
  assert(len <= rem);
  expose_cache();
  bmem_read_chunk(ptr, len);
  hide_cache();
  ptr += len;
  rem -= len;
}
static void move_cache_to_dst(void) {
  size_t s = BUF_SIZE - rem;
  if (s) {
    expose_cache();
    vmem_write_chunk(BUF_ADDR, s);
    hide_cache();
    vmem_skip(s);
    reset();
  }
}
static void dup_cache(uint16_t offset, size_t len) {
  expose_cache();
  memcpy(ptr, ptr - offset, len);
  hide_cache();
  ptr += len;
  rem -= len;
}

// -------------------------------------------------------
static void copy_src_to_dst(size_t len) {
  bmem_to_vmem(len);
}
static void dup_dst(uint16_t offset, size_t len) {
  vmem_dup(offset, len);
}

// -------------------------------------------------------
void vmem_buffer_open(void) {
  init();
}

void vmem_buffer_close(void) {
  move_cache_to_dst();
}

void bmem_to_vmem_buffer(size_t len) {
  if (rem < len) {
    move_cache_to_dst();
  }
  if (len <= rem) {
    copy_src_to_cache(len);
  }
  else {
    assert(rem == BUF_SIZE);
    copy_src_to_dst(len - rem); // direct transfer w/o cache
    copy_src_to_cache(rem);     // cache the remainder
  }
}

void vmem_buffer_dup(uint16_t offset, size_t len) {
  if (rem < len || BUF_SIZE < offset + rem) {
    move_cache_to_dst();
    dup_dst(offset, len);
  }
  else {
    dup_cache(offset, len);
  }
}

uint32_t ZX0_decompress_bmem_to_vmem_buffer(bmemptr_t src, vmemptr_t dst) {
  ZX0__fnptr_duplicate = vmem_buffer_dup;
  ZX0__fnptr_transfer  = bmem_to_vmem_buffer;
  ZX0__fnptr_read_byte = bmem_read_byte;
  bmem_open(src);
  vmem_open(dst);
  vmem_buffer_open();
  ZX0__decode();
  vmem_buffer_close();
  uint32_t s = vmem_ptr() - dst;
  vmem_close();
  bmem_close();
  return s;
}
