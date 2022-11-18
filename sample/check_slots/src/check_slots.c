// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2022 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file check_slots.c
 *
 * Print current slot for each pages, MAIN/SUB ROM slots, and MSX-MUSIC slots if
 * exists.
 */

#include <msx.h>
#include <stdint.h>

#include "screen1.h"

/**
 * Print the slot address.
 *
 * \param slot  the slot address
 */
void print_slot(uint8_t slot) {
  if (slot & 0x80) {
    puti(slot & 3);
    putc('-');
    puti((slot >> 2) & 3);
  }
  else {
    puti(slot & 3);
  }
}

/**
 * For each slot, invoke the given callback with the given arguments.
 *
 * \param callback  a user defined callback function that takes `slot` and `arg`.
 * \param arg       argument to be passed to the `callback`.
 */
void foreach_slot(void (*callback)(uint8_t slot, void * arg), void * arg) {
  for (uint8_t i = 0; i < 4; ++i) {
    uint8_t slot = i;
    if (EXPTBL[i] & 0x80) {
      slot |= 0x80;
      for (uint8_t j = 0; j < 4; ++j) {
        callback(slot | (j << 2), arg);
      }
    }
    else {
      callback(slot, arg);
    }
  }
}

/**
 * Compare the byte sequence to the one present at the given address in the given slot.
 *
 * \param slot,&nbsp;addr  a pair of slot address and address, of the one compared to.
 * \param s                pointer to the byte sequence.
 * \param len              length of the byte sequence.
 *
 * \return 0 if two byte sequence is same (or len was 0), otherwise non-zero value.
 */
int slot_bcmp(uint8_t slot, const void * addr, const void * s, size_t len) {
  const char * p = (const char *)addr;
  const char * q = (const char *)s;
  while (len--) {
    uint8_t c = msx_RDSLT(slot, (void *)p);
    __asm__("ei");
    if (c != *q) {
      return -1;
    }
    p++;
    q++;
  }
  return 0;
}

// -----------------------------------------------------------------------

static void find_internal_OPLL_callback(uint8_t slot, void * arg) {
  if (!slot_bcmp(slot, (const void *)0x4018, "APRLOPLL", 8)) {
    *((uint8_t *)arg) = slot;
  }
}

static void find_any_OPLL_callback(uint8_t slot, void * arg) {
  if (!slot_bcmp(slot, (const void *)0x401c, "OPLL", 4)) {
    *((uint8_t *)arg) = slot;
  }
}

/**
 * Find MSX-MUSIC.
 *
 * Searches for MSX-MUSIC (OPLL) and returns its slot address.
 *
 * First it searches for an internal MSX-MUSIC. Then, only if it is not found,
 * it searches for an external MSX-MUSIC. If not found, returns 0.
 *
 * \return slot address, or 0 if not found.
 */
uint8_t find_OPLL(void) {
  uint8_t slot = 0;
  foreach_slot(find_internal_OPLL_callback, &slot);
  if (!slot) {
    foreach_slot(find_any_OPLL_callback, &slot);
  }
  return slot;
}

// -----------------------------------------------------------------------

static void list_OPLL_callback(uint8_t slot, void * arg) {
  (void)arg;                    // unused
  if (!slot_bcmp(slot, (const void *)0x4018, "APRLOPLL", 8)) {
    puts(" (internal): "); print_slot(slot); putc('\n');
  }
  else if (!slot_bcmp(slot, (const void *)0x401c, "OPLL", 4)) {
    puts(" \"");
    for (uint8_t i = 0; i < 8; ++i) {
      putc(msx_RDSLT(slot, (void *)(uintptr_t)(0x4018 + i)));
    }
    __asm__("ei");
    puts("\": "); print_slot(slot); putc('\n');
  }
}

/**
 * Print a list of detected MSX-MUSIC.
 */
void list_OPLL(void) {
  puts("MSX-MUSIC  : slot"); putc('\n');
  foreach_slot(list_OPLL_callback, 0);
}

// -----------------------------------------------------------------------

struct z80_reg {
  uint16_t af;
  uint16_t bc;
  uint16_t de;
  uint16_t hl;
};

/**
 * Wrapper for BDOS call.
 * \param reg [inout]  pointer to value of CPU registers.
 */
void bdos(uint8_t c, struct z80_reg * reg) {
  (void)c;                      // A  --> C
  (void)reg;                    // de --> IX
  __asm__("push ix");
  __asm__("ld c, a");
  __asm__("push de");
  __asm__("pop ix");
  __asm__("ld a, 1 (ix)");
  __asm__("ld b, 3 (ix)");
  __asm__("ld e, 4 (ix)");
  __asm__("ld d, 5 (ix)");
  __asm__("ld l, 6 (ix)");
  __asm__("ld h, 7 (ix)");
  __asm__("push ix");
  __asm__("call 0xf37d");
  __asm__("pop ix");
  __asm__("ld 1 (ix), a");
  __asm__("ld 2 (ix), c");
  __asm__("ld 3 (ix), b");
  __asm__("ld 4 (ix), e");
  __asm__("ld 5 (ix), d");
  __asm__("ld 6 (ix), l");
  __asm__("ld 7 (ix), h");
  __asm__("pop ix");
}

// -----------------------------------------------------------------------

static volatile __at (0xfc9e) uint16_t JIFFY;

void main(void) {
  screen1();
  puts("CPU address: slot"); putc('\n');
  puts(" C000..FFFF: "); print_slot(msx_get_slot((void *)0xc000)); putc('\n');
  puts(" 8000..BFFF: "); print_slot(msx_get_slot((void *)0x8000)); putc('\n');
  puts(" 4000..7FFF: "); print_slot(msx_get_slot((void *)0x4000)); putc('\n');
  puts(" 0000..3FFF: "); print_slot(msx_get_slot((void *)0x0000)); putc('\n');
  putc('\n');
  puts("MSX BIOS   : slot"); putc('\n');
  puts(" MAIN ROM  : "); print_slot(EXPTBL[0]); putc('\n');
  if (0 < msx_get_version()) {
    // MSX2 or later
    puts(" SUB ROM   : "); print_slot(EXBRSA); putc('\n');
  }
  putc('\n');
  if (find_OPLL()) {
    list_OPLL();
  }

  putc('\n');
  if (DRVTBL[0]) {
    puts("FDC        : slot"); putc('\n');
    const volatile uint8_t * p = DRVTBL;
    while (*p) {
      puts("   "); puti(p[0]); puts(" drives: "); print_slot(p[1]);
      if (p[1] == MASTERS) {
        puts(" (master slot)");
      }
      putc('\n');
      p += 2;
    }

    struct z80_reg reg;
    bdos(0x6f, &reg);           // _DOSVER `DOS2` : Get MSX-DOS version
    if (!(reg.af >> 8) && 2 <= (reg.bc >> 8)) {
      // MSX-DOS 2.xx
      putc('\n');
      puts("MSX-DOS 2.x / Disk BASIC 2.x"); putc('\n');
      puts(" kernel version ");
      puti(reg.bc >> 8); putc('.'); puti((reg.bc >> 4) & 15); puti((reg.bc) & 15); putc('\n');
    }
    else {
      bdos(0x0c, &reg);         // _CPMVER `DOS1` `CP/M` : Get CP/M version
      if (reg.hl == 0x0022) {
        putc('\n');
        puts("MSX-DOS 1.x / Disk BASIC 1.x");
      }
    }
  }

  for (;;) {
    // await_vsync();
    locate(0,23);
    puts("TIME: "); puti(JIFFY); puts("     ");
  }
}
