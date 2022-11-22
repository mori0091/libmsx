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

#include "screen1.h"
#include "slot.h"
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
    puts(" (internal): "); print_slot(slot); putc('\n');
  }
  else if (slot_is_OPLL(slot)) {
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
  slot_iterate(list_OPLL_callback, 0);
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
  slot_iterate(list_SCC_callback, 0);
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
}

static void list_slot_callback(uint8_t slot, void * arg) {
  (void)arg;
  puts(" "); print_slot(slot); puts(" ");

  if (slot_is_MAIN_ROM(slot)) {
    puts("01__ MAIN ROM");
  }
  else if (slot_is_SUB_ROM(slot)) {
    puts("0___ SUB ROM");
  }
  else if (slot_is_RAM(slot)) {
    puts("0123 RAM");
  }
  else if (slot_is_internal_OPLL(slot)) {
    puts("_12_ MSX-MUSIC");
  }
  else if (slot_is_FMPAC(slot)) {
    puts("_12_ MSX-MUSIC (FMPAC)");
  }
  else if (slot_is_OPLL(slot)) {
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
  else if (slot_is_SCCPlus(slot)) {
    puts("_12_ SCC+ Sound Cartridge");
  }
  else if (slot_is_SCC(slot)) {
    puts("_12_ SCC MegaROM");
  }
  else if (slot_is_BDOS(slot)) {
    puts("_1__ BDOS/FDC");
  }
  else if (slot_is_ROM_p1(slot)) {
    if (slot_is_ROM_p2(slot)) {
      // probably mirrored
      puts("_1__ 16K ROM");
    }
    else {
      puts("_12_ 32K ROM");
    }
  }
  else if (slot_is_ROM_p2(slot)) {
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
  locate(0, 22); puts("Playing test tone with SCC/SCC+");
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

  if (SCC_find(&scc)) {
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
    test_SCC();
    show_timer_loop();
    stop_SCC();
  }
}
