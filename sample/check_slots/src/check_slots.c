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
#include <string.h>
#include <stdio.h>

#include "screen.h"
#include "text.h"

#include "slot.h"
#include "scc.h"
#include "bdos.h"

/**
 * Print the slot address.
 *
 * \param slot  the slot address
 */
void print_slot(uint8_t slot) {
  printi(slot & 3);
  if (slot & 0x80) {
    putchar('-');
    printi((slot >> 2) & 3);
  }
  else {
    print("  ");
  }
}

// -----------------------------------------------------------------------

static void find_internal_OPLL_callback(uint8_t slot, void * arg) {
  if (slot_is_internal_OPLL(slot)) {
    *((uint8_t *)arg) = slot;
  }
}

static void find_any_OPLL_callback(uint8_t slot, void * arg) {
  if (slot_is_OPLL(slot)) {
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
  slot_iterate(find_internal_OPLL_callback, &slot);
  if (!slot) {
    slot_iterate(find_any_OPLL_callback, &slot);
  }
  return slot;
}

// -----------------------------------------------------------------------

static void list_OPLL_callback(uint8_t slot, void * arg) {
  (void)arg;                    // unused
  if (slot_is_internal_OPLL(slot)) {
    print(" (internal): "); print_slot(slot); newline();
  }
  else if (slot_is_OPLL(slot)) {
    print(" \"");
    for (uint8_t i = 0; i < 8; ++i) {
      putchar(msx_RDSLT(slot, (void *)(uintptr_t)(0x4018 + i)));
    }
    __asm__("ei");
    print("\": "); print_slot(slot); newline();
  }
}

/**
 * Print a list of detected MSX-MUSIC.
 */
void list_OPLL(void) {
  print("MSX-MUSIC  : slot\n");
  slot_iterate(list_OPLL_callback, 0);
}

// -----------------------------------------------------------------------

static void list_SCC_callback(uint8_t slot, void * arg) {
  (void)arg;                    // unused
  uint8_t scc_ver = SCC_inspect(slot);
  if (1 < scc_ver) {
    print(" SCC+      : "); print_slot(slot); newline();
  }
  else if (0 < scc_ver) {
    print(" SCC       : "); print_slot(slot); newline();
  }
}

/**
 * Print a list of detected Konami SCC/SCC+.
 */
void list_SCC(void) {
  print("Konami SCC : slot\n");
  slot_iterate(list_SCC_callback, 0);
}

// -----------------------------------------------------------------------

void list_BDOS(void) {
  print("BDOS/FDC   : slot\n");
  const volatile uint8_t * p = DRVTBL;
  while (*p) {
    printf(" %d drives  : ", (int)p[0]); print_slot(p[1]);
    // print(" "); printi(p[0]); print(" drives  : "); print_slot(p[1]);
    if (p[1] == MASTERS) {
      print(" (master slot)");
    }
    newline();
    p += 2;
  }
}

void print_DOS_version(void) {
  struct z80_reg reg;
  bdos(0x6f, &reg);           // _DOSVER `DOS2` : Get MSX-DOS version
  if (!(reg.af >> 8) && 2 <= (reg.bc >> 8)) {
    // MSX-DOS 2.xx
    newline();
    printf("MSX-DOS kernel version %x.%02x\n", reg.bc >> 8, reg.bc & 255);
  }
  else {
    bdos(0x0c, &reg);         // _CPMVER `DOS1` `CP/M` : Get CP/M version
    if (reg.hl == 0x0022) {
      newline();
      print("MSX-DOS kernel version 1.x\n");
    }
  }
}

// -----------------------------------------------------------------------

/**
 * Print the current memory map of CPU address space.
 */
void show_memory_map(void) {
  cls();
  print("CPU address slot\n");
  print("----------- ----\n");
  print(" C000..FFFF "); print_slot(msx_get_slot((void *)0xc000)); newline();
  print(" 8000..BFFF "); print_slot(msx_get_slot((void *)0x8000)); newline();
  print(" 4000..7FFF "); print_slot(msx_get_slot((void *)0x4000)); newline();
  print(" 0000..3FFF "); print_slot(msx_get_slot((void *)0x0000)); newline();
  newline();
}

static void list_slot_callback(uint8_t slot, void * arg) {
  (void)arg;
  print(" "); print_slot(slot); print(" ");

  if (slot_is_MAIN_ROM(slot)) {
    print("01__ MAIN ROM");
  }
  else if (slot_is_SUB_ROM(slot)) {
    print("0___ SUB ROM");
  }
  else if (slot_is_RAM(slot)) {
    print("0123 RAM");
  }
  else if (slot_is_internal_OPLL(slot)) {
    print("_12_ MSX-MUSIC");
  }
  else if (slot_is_FMPAC(slot)) {
    print("_12_ MSX-MUSIC (FMPAC)");
  }
  else if (slot_is_OPLL(slot)) {
    print("_12_ MSX-MUSIC");
    if (slot_bcmp(slot, (const void *)0x4018, "APRL", 4)) {
      print(" (");
      const char * p = (const char *)0x4018;
      for (uint8_t i = 4; i--; ) {
        putchar(msx_RDSLT(slot, (void *)p++));
      }
      __asm__("ei");
      putchar(')');
    }
  }
  else if (slot_is_SCCPlus(slot)) {
    print("_12_ SCC+ Sound Cartridge");
  }
  else if (slot_is_SCC(slot)) {
    print("_12_ SCC MegaROM");
  }
  else if (slot_is_BDOS(slot)) {
    print("_1__ BDOS/FDC");
  }
  else if (slot_is_ROM_p1(slot)) {
    if (slot_is_ROM_p2(slot)) {
      // probably mirrored
      print("_1__ 16K ROM");
    }
    else {
      print("_12_ 32K ROM");
    }
  }
  else if (slot_is_ROM_p2(slot)) {
    print("__2_ 16K ROM");
  }
  else {
    print("____ ");
  }
  newline();
}

/**
 * Prints a list of inspection results for each slot.
 */
void list_slots(void) {
  print("slot page\n");
  print("---- ---- ---------------------\n");
  slot_iterate(list_slot_callback, 0);
}

// -----------------------------------------------------------------------

const int8_t triangle[32] = {
  0, 16, 32, 48, 64, 80, 96, 112,
  127, 112, 96, 80, 64, 48, 32, 16,
  0, -16, -32, -48, -64, -80, -96, -112,
  -128, -112, -96, -80, -64, -48, -32, -16,
};

static struct SCC scc;

void test_SCC(void) {
  if (!scc.slot) return;
  SCC_enable(&scc);
  {
    uint8_t slot_p2 = msx_get_slot((void *)0x8000);
    msx_ENASLT(scc.slot, (void *)0x8000);
    {
      memcpy((void *)scc.device->channels[0].wo_waveform, triangle, 32);
      *scc.device->rw_channel_mask = 0x01; // unmute ch1
      *scc.device->channels[0].rw_volume = 15;
      *scc.device->channels[0].rw_fdr = 0x11d; // O4 G
    }
    msx_ENASLT(slot_p2, (void *)0x8000);
    __asm__("ei");
  }
  SCC_disable(&scc);
  locate(0, 22);
  color(6, 10, 4);
  print("Playing test tone with SCC/SCC+ ");
  color(15, 4, 4);
}

void stop_SCC(void) {
  if (!scc.slot) return;
  SCC_enable(&scc);
  {
    uint8_t slot_p2 = msx_get_slot((void *)0x8000);
    msx_ENASLT(scc.slot, (void *)0x8000);
    {
      *scc.device->rw_channel_mask = 0x00; // mute all channels
      *scc.device->channels[0].rw_volume = 0;
      *scc.device->channels[1].rw_volume = 0;
      *scc.device->channels[2].rw_volume = 0;
      *scc.device->channels[3].rw_volume = 0;
      *scc.device->channels[4].rw_volume = 0;
    }
    msx_ENASLT(slot_p2, (void *)0x8000);
    __asm__("ei");
  }
  SCC_disable(&scc);
}

void show_system_environment(void) {
  cls();
  print("MSX BIOS   : slot\n");
  print(" MAIN ROM  : "); print_slot(EXPTBL[0]); newline();
  if (0 < msx_get_version()) {
    // MSX2 or later
    print(" SUB ROM   : "); print_slot(EXBRSA); newline();
  }
  newline();
  print("MSX FONT   : slot\n");
  print(" ROM FONT  : "); print_slot(EXPTBL[0]);  putchar(' ');
  printf("%04X\n", (const uintptr_t)CGTBL);
  print(" (current) : "); print_slot(CGPNT.slot); putchar(' ');
  printf("%04X\n", (const uintptr_t)CGPNT.addr);

  if (find_OPLL()) {
    newline();
    list_OPLL();
  }

  if (SCC_find(&scc)) {
    newline();
    list_SCC();
  }

  if (DRVTBL[0]) {
    newline();
    list_BDOS();
    print_DOS_version();
  }
}

// -----------------------------------------------------------------------

void show_timer(void) {
  locate(0,23);
  printf("TIME:%5u                      ", JIFFY);
}

void show_timer_loop(void) {
  color(15, 1, 4);
  const uint16_t n = 5 * msx_get_vsync_frequency();
  const uint16_t t = JIFFY;
  for (; JIFFY - t < n; ) {
    await_vsync();
    show_timer();
  }
  color(15, 4, 4);
}

void main(void) {
  screen2();
  color(15, 4, 4);
  cls();

  for (;;) {
    show_memory_map();
    list_slots();
    show_timer_loop();

    show_system_environment();
    test_SCC();
    show_timer_loop();
    stop_SCC();
  }
}
