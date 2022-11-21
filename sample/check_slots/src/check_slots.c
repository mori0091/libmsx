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
#include "scc.h"
#include "bdos.h"

/**
 * Clear screen.
 */
void cls(void) {
  clear_vmem();
  locate(0, 0);
}

/**
 * Print the slot address.
 *
 * \param slot  the slot address
 */
void print_slot(uint8_t slot) {
  puti(slot & 3);
  if (slot & 0x80) {
    putc('-');
    puti((slot >> 2) & 3);
  }
  else {
    puts("  ");
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

bool is_MAIN_ROM(uint8_t slot) {
  return (slot == EXPTBL[0]);
}

bool is_SUB_ROM(uint8_t slot) {
  return (EXBRSA && slot == EXBRSA || !slot_bcmp(slot, (const void *)0x0000, "CD", 2));
}

bool is_RAM(uint8_t slot) {
  return (slot == msx_get_slot((void *)0xc000));
}

bool is_internal_OPLL(uint8_t slot) {
  return (!slot_bcmp(slot, (const void *)0x4018, "APRLOPLL", 8));
}

bool is_FMPAC(uint8_t slot) {
  return (!slot_bcmp(slot, (const void *)0x4018, "PAC2OPLL", 8));
}

bool is_OPLL(uint8_t slot) {
  return (!slot_bcmp(slot, (const void *)0x401c, "OPLL", 4));
}

bool is_SCC(uint8_t slot) {
  return (0 < SCC_inspect(slot));
}

bool is_SCCPlus(uint8_t slot) {
  return (1 < SCC_inspect(slot));
}

bool is_BDOS(uint8_t slot) {
  if (!DRVTBL[0]) return false;
  const volatile uint8_t * p = DRVTBL;
  while (*p) {
    if (slot == p[1]) {
      return true;
    }
    p += 2;
  }
  return false;
}

bool is_ROM_p1(uint8_t slot) {
  return (!slot_bcmp(slot, (const void *)0x4000, "AB", 2));
}

bool is_ROM_p2(uint8_t slot) {
  return (!slot_bcmp(slot, (const void *)0x8000, "AB", 2));
}

bool is_ROM(uint8_t slot) {
  return (is_ROM_p1(slot) || is_ROM_p2(slot));
}

bool is_read_only(uint8_t slot, void * addr) {
  const uint8_t x = msx_RDSLT(slot, addr);
  msx_WRSLT(slot, addr, ~x);
  if (!(x ^ msx_RDSLT(slot, addr))) {
    __asm__("ei");
    return true;
  }
  msx_WRSLT(slot, addr, x);
  __asm__("ei");
  return false;
}

// -----------------------------------------------------------------------

static void find_internal_OPLL_callback(uint8_t slot, void * arg) {
  if (is_internal_OPLL(slot)) {
    *((uint8_t *)arg) = slot;
  }
}

static void find_any_OPLL_callback(uint8_t slot, void * arg) {
  if (is_OPLL(slot)) {
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
  if (is_internal_OPLL(slot)) {
    puts(" (internal): "); print_slot(slot); putc('\n');
  }
  else if (is_OPLL(slot)) {
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

static void list_SCC_callback(uint8_t slot, void * arg) {
  (void)arg;                    // unused
  uint8_t scc_ver = SCC_inspect(slot);
  if (1 < scc_ver) {
    puts(" SCC+      : "); print_slot(slot); putc('\n');
  }
  else if (0 < scc_ver) {
    puts(" SCC       : "); print_slot(slot); putc('\n');
  }
}

/**
 * Print a list of detected Konami SCC/SCC+.
 */
void list_SCC(void) {
  puts("Konami SCC : slot"); putc('\n');
  foreach_slot(list_SCC_callback, 0);
}

// -----------------------------------------------------------------------

void list_BDOS(void) {
  puts("BDOS/FDC   : slot"); putc('\n');
  const volatile uint8_t * p = DRVTBL;
  while (*p) {
    puts(" "); puti(p[0]); puts(" drives  : "); print_slot(p[1]);
    if (p[1] == MASTERS) {
      puts(" (master slot)");
    }
    putc('\n');
    p += 2;
  }
}

void print_DOS_version(void) {
  struct z80_reg reg;
  bdos(0x6f, &reg);           // _DOSVER `DOS2` : Get MSX-DOS version
  if (!(reg.af >> 8) && 2 <= (reg.bc >> 8)) {
    // MSX-DOS 2.xx
    putc('\n');
    puts("MSX-DOS kernel version ");
    puti(reg.bc >> 8); putc('.'); puti((reg.bc >> 4) & 15); puti((reg.bc) & 15); putc('\n');
  }
  else {
    bdos(0x0c, &reg);         // _CPMVER `DOS1` `CP/M` : Get CP/M version
    if (reg.hl == 0x0022) {
      putc('\n');
      puts("MSX-DOS kernel version 1.x"); putc('\n');
    }
  }
}

// -----------------------------------------------------------------------

static void list_slot_callback(uint8_t slot, void * arg);

/**
 * Print the current memory map of CPU address space.
 */
void show_memory_map(void) {
  cls();
  puts("CPU address slot"); putc('\n');
  puts("----------- ----"); putc('\n');
  puts(" C000..FFFF "); print_slot(msx_get_slot((void *)0xc000)); putc('\n');
  puts(" 8000..BFFF "); print_slot(msx_get_slot((void *)0x8000)); putc('\n');
  puts(" 4000..7FFF "); print_slot(msx_get_slot((void *)0x4000)); putc('\n');
  puts(" 0000..3FFF "); print_slot(msx_get_slot((void *)0x0000)); putc('\n');
  putc('\n');
  // puts("slot page"); putc('\n');
  // puts("---- ---- ----------------"); putc('\n');
  // foreach_slot(list_slot_callback, 0);
}

static void list_slot_callback(uint8_t slot, void * arg) {
  (void)arg;
  puts(" "); print_slot(slot); puts(" ");

  if (is_MAIN_ROM(slot)) {
    puts("01__ MAIN ROM");
  }
  else if (is_SUB_ROM(slot)) {
    puts("0___ SUB ROM");
  }
  else if (is_RAM(slot)) {
    puts("0123 RAM");
  }
  else if (is_internal_OPLL(slot)) {
    puts("_12_ MSX-MUSIC");
  }
  else if (is_FMPAC(slot)) {
    puts("_12_ MSX-MUSIC (FMPAC)");
  }
  else if (is_OPLL(slot)) {
    puts("_12_ MSX-MUSIC");
    if (slot_bcmp(slot, (const void *)0x4018, "APRL", 4)) {
      puts(" (");
      const char * p = (const char *)0x4018;
      for (uint8_t i = 4; i--; ) {
        putc(msx_RDSLT(slot, (void *)p++));
      }
      __asm__("ei");
      putc(')');
    }
  }
  else if (is_SCCPlus(slot)) {
    puts("_12_ SCC+ Sound Cartridge");
  }
  else if (is_SCC(slot)) {
    puts("_12_ SCC MegaROM");
  }
  else if (is_BDOS(slot)) {
    puts("_1__ BDOS/FDC");
  }
  else if (is_ROM_p1(slot)) {
    if (is_ROM_p2(slot)) {
      // probably mirrored
      puts("_1__ 16K ROM");
    }
    else {
      puts("_12_ 32K ROM");
    }
  }
  else if (is_ROM_p2(slot)) {
    puts("__2_ 16K ROM");
  }
  else {
    puts("____ ");
  }
  putc('\n');
}

/**
 * Prints a list of inspection results for each slot.
 */
void list_slots(void) {
  puts("slot page"); putc('\n');
  puts("---- ---- ----------------"); putc('\n');
  foreach_slot(list_slot_callback, 0);
}

// -----------------------------------------------------------------------

void show_system_environment(void) {
  cls();
  puts("MSX BIOS   : slot"); putc('\n');
  puts(" MAIN ROM  : "); print_slot(EXPTBL[0]); putc('\n');
  if (0 < msx_get_version()) {
    // MSX2 or later
    puts(" SUB ROM   : "); print_slot(EXBRSA); putc('\n');
  }
  if (find_OPLL()) {
    putc('\n');
    list_OPLL();
  }

  struct SCC scc;
  SCC_find(&scc);
  if (scc.slot) {
    putc('\n');
    list_SCC();
  }

  if (DRVTBL[0]) {
    putc('\n');
    list_BDOS();
    print_DOS_version();
  }
}

// -----------------------------------------------------------------------

static volatile __at (0xfc9e) uint16_t JIFFY;

void show_timer(void) {
  locate(0,23);
  puts("TIME: "); puti(JIFFY); puts("     ");
}

void show_timer_loop(void) {
  uint16_t t = 5 * msx_get_vsync_frequency();
  while (t--) {
    await_vsync();
    show_timer();
  }
}

void main(void) {
  screen1();

  for (;;) {
    show_memory_map();
    list_slots();
    show_timer_loop();

    show_system_environment();
    show_timer_loop();
  }
}
