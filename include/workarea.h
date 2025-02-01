// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2025 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file workarea.h
 * \brief Definitions of MSX SYSTEM Workarea.
 */

#pragma once

#ifndef WORKAREA_H
#define WORKAREA_H

#include <stdint.h>

#include "config.h"
#include "bios_const.h"

// ---- BDOS/FDC workarea ----
/**
 * `MSX` Pointer to pointer to disk error routine.
 */
static volatile __at (0xf323) void ** DISKVE;

/**
 * `MSX` Pointer to pointer to `CTRL`+`C` routine.
 */
static volatile __at (0xf325) void ** BREAKV;

/**
 * `MSX` Slot address of page #0 of RAM.
 *
 * \note
 * Available only if a Floppy Disk Controller (FDC) exists and was initialized
 * at system startup.
 */
static volatile __at (0xf341) uint8_t RAMAD0;

/**
 * `MSX` Slot address of page #1 of RAM.
 *
 * \note
 * Available only if a Floppy Disk Controller (FDC) exists and was initialized
 * at system startup.
 */
static volatile __at (0xf342) uint8_t RAMAD1;

/**
 * `MSX` Slot address of page #2 of RAM.
 *
 * \note
 * Available only if a Floppy Disk Controller (FDC) exists and was initialized
 * at system startup.
 */
static volatile __at (0xf343) uint8_t RAMAD2;

/**
 * `MSX` Slot address of page #3 of RAM.
 *
 * \note
 * Available only if a Floppy Disk Controller (FDC) exists and was initialized
 * at system startup.
 */
static volatile __at (0xf344) uint8_t RAMAD3;

/**
 * `MSX` Slot address of the master FDC cartridge.
 *
 * \note
 * Available only if a Floppy Disk Controller (FDC) exists and was initialized
 * at system startup.
 */
static volatile __at (0xf348) uint8_t MASTERS;

// ---- sub-routine for inter-slot call  ----
static volatile __at (0xf380) uint8_t RDPRIM[5]; ///< `MSX` Inter-slot read from primary slot.
static volatile __at (0xf385) uint8_t WRPRIM[7]; ///< `MSX` Inter-slot write to primary slot.
static volatile __at (0xf38c) uint8_t CLPRIM[14]; ///< `MSX` Inter-slot call to primary slot.

// ---- USR function start address / text screen ----
static volatile __at (0xf39a) void * USRTAB[10]; ///< `MSX` Table of start address of `USR0`..`USR9`.

static volatile __at (0xf3ae) uint8_t LINL40; ///< `MSX` WIDTH in SCREEN 0
static volatile __at (0xf3af) uint8_t LINL32; ///< `MSX` WIDTH in SCREEN 1
static volatile __at (0xf3b0) uint8_t LINLEN; ///< `MSX` WIDTH of the current screen.
static volatile __at (0xf3b1) uint8_t CRTCNT; ///< `MSX` Number of lines of the current screen.
static volatile __at (0xf3b2) uint8_t CLMLST; ///< `MSX` Used for `PRINT` statement.

// ---- VRAM memory map / other screen setting ----
static volatile __at (0xf3b3) uint16_t TXTNAM; ///< `MSX` SCREEN 0 / Pattern name table address.
static volatile __at (0xf3b5) uint16_t TXTCOL; ///< `MSX2` SCREEN 0 / Color table address.
static volatile __at (0xf3b7) uint16_t TXTCGP; ///< `MSX` SCREEN 0 / Pattern generator table address.
static volatile __at (0xf3b9) uint16_t TXTATR; ///< (unused)
static volatile __at (0xf3bb) uint16_t TXTPAT; ///< (unused)

static volatile __at (0xf3bd) uint16_t T32NAM; ///< `MSX` SCREEN 1 / Pattern name table address.
static volatile __at (0xf3bf) uint16_t T32COL; ///< `MSX` SCREEN 1 / Color table address.
static volatile __at (0xf3c1) uint16_t T32CGP; ///< `MSX` SCREEN 1 / Pattern generator table address.
static volatile __at (0xf3c3) uint16_t T32ATR; ///< `MSX` SCREEN 1 / Sprite attribute table address.
static volatile __at (0xf3c5) uint16_t T32PAT; ///< `MSX` SCREEN 1 / Sprite pattern generator table address.

static volatile __at (0xf3c7) uint16_t GRPNAM; ///< `MSX` SCREEN 2 / Pattern name table address.
static volatile __at (0xf3c9) uint16_t GRPCOL; ///< `MSX` SCREEN 2 / Color table address.
static volatile __at (0xf3cb) uint16_t GRPCGP; ///< `MSX` SCREEN 2 / Pattern generator table address.
static volatile __at (0xf3cd) uint16_t GRPATR; ///< `MSX` SCREEN 2 / Sprite attribute table address.
static volatile __at (0xf3cf) uint16_t GRPPAT; ///< `MSX` SCREEN 2 / Sprite pattern generator table address.

static volatile __at (0xf3d1) uint16_t MLTNAM; ///< `MSX` SCREEN 3 / Pattern name table address.
static volatile __at (0xf3d3) uint16_t MLTCOL; ///< (unused)
static volatile __at (0xf3d5) uint16_t MLTCGP; ///< `MSX` SCREEN 3 / Pattern generator table address.
static volatile __at (0xf3d7) uint16_t MLTATR; ///< `MSX` SCREEN 3 / Sprite attribute table address.
static volatile __at (0xf3d9) uint16_t MLTPAT; ///< `MSX` SCREEN 3 / Sprite pattern generator table address.

// ---- BASIC ----
static volatile __at (0xf3db) uint8_t CLIKSW; ///< `MSX` Key click beep switch (0:off, non-zero:on).
static volatile __at (0xf3dc) uint8_t CSRY;   ///< `MSX` Y coordinate of cursor.
static volatile __at (0xf3dd) uint8_t CSRX;   ///< `MSX` X coordinate of cursor.
static volatile __at (0xf3de) uint8_t CNSDFG; ///< `MSX` Function Key display switch (0:off, non-zero:on).

// ---- VDP (TMS9918) register save area, etc. ----
static volatile __at (0xf3df) uint8_t RG0SAV; ///< `MSX` Saved value for VDP R#0 register.
static volatile __at (0xf3e0) uint8_t RG1SAV; ///< `MSX` Saved value for VDP R#1 register.
static volatile __at (0xf3e1) uint8_t RG2SAV; ///< `MSX` Saved value for VDP R#2 register.
static volatile __at (0xf3e2) uint8_t RG3SAV; ///< `MSX` Saved value for VDP R#3 register.
static volatile __at (0xf3e3) uint8_t RG4SAV; ///< `MSX` Saved value for VDP R#4 register.
static volatile __at (0xf3e4) uint8_t RG5SAV; ///< `MSX` Saved value for VDP R#5 register.
static volatile __at (0xf3e5) uint8_t RG6SAV; ///< `MSX` Saved value for VDP R#6 register.
static volatile __at (0xf3e6) uint8_t RG7SAV; ///< `MSX` Saved value for VDP R#7 register.

static volatile __at (0xf3e7) uint8_t STATFL; ///< `MSX` Saved value for VDP S#0 register.

// ---- BASIC ----
static volatile __at (0xf3e8) uint8_t TRGFLG; ///< `MSX` Joystick trigger button state.

static volatile __at (0xf3e9) uint8_t FORCLR; ///< `MSX` Foreground color.
static volatile __at (0xf3ea) uint8_t BAKCLR; ///< `MSX` Background color.
static volatile __at (0xf3eb) uint8_t BDRCLR; ///< `MSX` Border color.

static volatile __at (0xf3ec) uint8_t MAXUPD[3]; ///< `MSX` Internal use of `LINE` statement.
static volatile __at (0xf3ef) uint8_t MINUPD[3]; ///< `MSX` Internal use of `LINE` statement.
static volatile __at (0xf3f2) uint8_t ATRBYT; ///< `MSX` Color in graphic mode / attribute byte.

static volatile __at (0xf3f3) void * QUEUES; ///< `MSX` Pointer to the queue table of `PLAY` statement.

static volatile __at (0xf3f5) uint8_t FRCNEW; ///< `MSX` Internal use of BASIC interpreter.
static volatile __at (0xf3f6) uint8_t SCNCNT; ///< `MSX` Key scanning interval time.
static volatile __at (0xf3f7) uint8_t REPCNT; ///< `MSX` Interval time until auto repeat of key input.
static volatile __at (0xf3f8) void *  PUTPNT; ///< `MSX` Pointer to key buffer for writing.
static volatile __at (0xf3fa) void *  GETPNT; ///< `MSX` Pointer to key buffer for reading.

// ---- Cassette Tape, etc. ----
// f3fc .. f413

// ---- Internal use for BASIC ----
// f414 .. f6e3

// ---- Parameters for user functions (BASIC) ----
// f6e4 .. f7c4

// ---- Math-pack ----
// f7c5 .. f85e

// ---- Data for BASIC ----
// f85f .. f930

/**
 * `MSX` String for each function keys.
 */
static volatile __at (0xf87f) uint8_t FNKSTR[10][16];

/**
 * `MSX` Structure of `CGPNT`.
 * \sa CGPNT
 */
struct cgpnt {
  uint8_t slot;                 ///< Slot of the current fonts.
  struct font * addr;           ///< Address of the current fonts.
};

/**
 * `MSX` Location of the current fonts.
 *
 * ~~~ c
 * CGPNT.slot; // Slot of the current fonts.
 * CGPNT.addr; // Address of the current fonts.
 * ~~~
 */
static volatile __at (0xf91f) struct cgpnt const CGPNT;

static volatile __at (0xf922) uint16_t NAMBAS; ///< `MSX` Current pattern name table address.
static volatile __at (0xf924) uint16_t CGPBAS; ///< `MSX` Current pattern generator table address.
static volatile __at (0xf926) uint16_t PATBAS; ///< `MSX` Current sprite pattern generator table address.
static volatile __at (0xf928) uint16_t ATRBAS; ///< `MSX` Current sprite attribute table address.


// ---- Data for `CIRCLE`/`PAINT` statements ----
// f931 .. f955

// ---- Data for `PLAY` statement ----
// f956 .. faf4

// ---- Workarea for `MSX2` or later ----
// faf5 .. fb02
/**
 * `MSX2` The slot address of SUB ROM
 *
 * \note In case of `MSX1`, there is no SUB ROM and `EXBRSA == 0`.
 */
static volatile __at (0xfaf8) uint8_t EXBRSA;

// ---- Data for RS-232C, etc. ----
// fb03 .. fb28

static volatile __at (0xfb20) uint8_t HOKVLD; ///< `MSX` Extended BIOS flag.

/**
 * `MSX` Slot and number of drives for each Floppy Disk Controller (FDC).
 *
 * | DRVTBL[*] | explanation                     |
 * | --------- | ------------------------------- |
 * | DRVTBL[0] | Number of drives of the 1st FDC |
 * | DRVTBL[1] | Slot address of the 1st FDC     |
 * | DRVTBL[2] | Number of drives of the 2nd FDC |
 * | DRVTBL[3] | Slot address of the 2nd FDC     |
 * | DRVTBL[4] | Number of drives of the 3rd FDC |
 * | DRVTBL[5] | Slot address of the 3rd FDC     |
 * | DRVTBL[6] | Number of drives of the 4th FDC |
 * | DRVTBL[7] | Slot address of the 4th FDC     |
 *
 * \note
 * Available only if a Floppy Disk Controller (FDC) exists and was initialized
 * at system startup.
 */
static volatile __at (0xfb21) uint8_t DRVTBL[8];

/**
 * `MSX` The highest address of free area (bottom of stack).
 */
static volatile __at (0xfc4a) uint16_t HIMEM;

// ---- Data for `PLAY` statement ----
// fb35 .. fbaf

// ---- Data area ----
// fbb0 .. fd99

// ------ Timer ------
/**
 * `MSX` Free-running counter that counts up every VSYNC interrupts.
 */
static volatile __at (0xfc9e) uint16_t JIFFY;

// ------ Slot workarea ------
/**
 * `MSX` Expansion flags for each primary slots.
 *
 * The bit #7 of EXPTBL[i] indicates whether the primary slot \#i is expanded or
 * not. If the bit #7 was `1`, the primary slot \#i is expanded to 4 secondary
 * slots. Otherwise, the primary slot \#i is single slot.
 *
 * And the EXPTBL[0] is also indicates actual MAIN ROM slot address.
 * For any other EXPTBL[i], only bit #7 is used.
 *
 * A slot address (or 'slot' in short) is a 8 bit value consists of the following:
 * - bit #7     : expanded (1) or not (0)
 * - bit #6..#4 : unused
 * - bit #3..#2 : secondary slot number (0..3) if bit #7 was 1
 * - bit #1..#0 : primary slot number (0..3)
 */
static volatile __at (0xfcc1) uint8_t EXPTBL[4];

/**
 * `MSX` Save area for expanded slot select registers.
 *
 * SLTTBL[i] is saved value of expanded slot select register of the primary slot \#i.
 *
 * Value of a expanded slot select register is a 8 bit value consists of the following:
 * - bit #7..#6 : secondary slot number of page #3
 * - bit #5..#4 : secondary slot number of page #2
 * - bit #3..#2 : secondary slot number of page #1
 * - bit #1..#0 : secondary slot number of page #0
 */
static volatile __at (0xfcc5) uint8_t SLTTBL[4];

/**
 * `MSX` Attributes for application (cartridge).
 *
 * For each slot, indicates which pages contain or do not contain MSX-BASIC
 * program text, MSX-BASIC extended device routines, and/or MSX-BASIC extended
 * statements.
 *
 * There is 1 byte for each page of each slots. For page \#p of slot \#m-n,
 * `SLTATR[i]` is used where `i = 16*m + 4*n + p`.
 *
 * | SLTATR[*]  | corresponding page/slot  |
 * | ---------- | ------------------------ |
 * | SLTATR[ 0] | for page #0 of slot #0-0 |
 * | SLTATR[ 2] | for page #1 of slot #0-0 |
 * | ...        | ...                      |
 * | SLTATR[62] | for page #2 of slot #3-3 |
 * | SLTATR[63] | for page #3 of slot #3-3 |
 *
 * Each `SLTATR[i]` is a 8 bit value consists of the following:
 * - bit #7     : MSX-BASIC program TEXT exists (1), or not (0)
 * - bit #6     : MSX-BASIC extended DEVICE routine exists (1), or not (0)
 * - bit #5     : MSX-BASIC extended STATEMENT exists (1), or not (0)
 * - bit #4..#0 : unused
 */
static volatile __at (0xfcc9) uint8_t SLTATR[64];

/**
 * `MSX` Workarea for applications (cartridge).
 *
 * There is 2 bytes for each page of each slots. For page \#p of slot \#m-n,
 * `SLTWRK[2*i]` and `SLTWRK[2*i+1]` are used where `i = 16*m + 4*n + p`.
 *
 * | SLTWRK[*]                   | corresponding page/slot  |
 * | --------------------------- | ------------------------ |
 * | SLTWRK[  0] and SLTWRK[  1] | for page #0 of slot #0-0 |
 * | SLTWRK[  2] and SLTWRK[  3] | for page #1 of slot #0-0 |
 * | ...                         | ...                      |
 * | SLTWRK[124] and SLTWRK[125] | for page #2 of slot #3-3 |
 * | SLTWRK[126] and SLTWRK[127] | for page #3 of slot #3-3 |
 */
static volatile __at (0xfd09) uint8_t SLTWRK[128];


// ---- Hooks (interrupts, console I/O, etc.) ----
// fd9a .. fdee
static volatile __at(0xfd9a) uint8_t H_KEYI[5]; ///< Hook for interrupts.
static volatile __at(0xfd9f) uint8_t H_TIMI[5]; ///< Hook for timer interrupts (VSYNC).

// ---- Hooks (disk I/O) ----
// fdef .. feb1

// ---- Hooks (logical device name) ----
// feb2 .. feca

// ---- Hooks (internal use for BASIC) ----
// fecb .. ffc9

// ---- Extended BIOS ----
static volatile __at (0xffca) uint8_t FCALL[5];  ///< `MSX` Used by extended BIOS.
static volatile __at (0xffcf) uint8_t DISINT[5]; ///< `MSX` Used by DOS.
static volatile __at (0xffd4) uint8_t ENAINT[5]; ///< `MSX` Used by DOS


// ---- VDP (V9938) register save area. ----
static volatile __at (0xffe7) uint8_t RG8SAV; ///< `MSX2` Saved value for VDP R#8 register.
static volatile __at (0xffe8) uint8_t RG9SAV; ///< `MSX2` Saved value for VDP R#9 register.
static volatile __at (0xffe9) uint8_t RG10SA; ///< `MSX2` Saved value for VDP R#10 register.
static volatile __at (0xffea) uint8_t RG11SA; ///< `MSX2` Saved value for VDP R#11 register.
static volatile __at (0xffeb) uint8_t RG12SA; ///< `MSX2` Saved value for VDP R#12 register.
static volatile __at (0xffec) uint8_t RG13SA; ///< `MSX2` Saved value for VDP R#13 register.
static volatile __at (0xffed) uint8_t RG14SA; ///< `MSX2` Saved value for VDP R#14 register.
static volatile __at (0xffee) uint8_t RG15SA; ///< `MSX2` Saved value for VDP R#15 register.
static volatile __at (0xffef) uint8_t RG16SA; ///< `MSX2` Saved value for VDP R#16 register.
static volatile __at (0xfff0) uint8_t RG17SA; ///< `MSX2` Saved value for VDP R#17 register.
static volatile __at (0xfff1) uint8_t RG18SA; ///< `MSX2` Saved value for VDP R#18 register.
static volatile __at (0xfff2) uint8_t RG19SA; ///< `MSX2` Saved value for VDP R#19 register.
static volatile __at (0xfff3) uint8_t RG20SA; ///< `MSX2` Saved value for VDP R#20 register.
static volatile __at (0xfff4) uint8_t RG21SA; ///< `MSX2` Saved value for VDP R#21 register.
static volatile __at (0xfff5) uint8_t RG22SA; ///< `MSX2` Saved value for VDP R#22 register.
static volatile __at (0xfff6) uint8_t RG23SA; ///< `MSX2` Saved value for VDP R#23 register.

// ---- VDP (V9958) register save area. ----
static volatile __at (0xfffa) uint8_t RG25SA; ///< `MSX2+` Saved value for VDP R#25 register.
static volatile __at (0xfffb) uint8_t RG26SA; ///< `MSX2+` Saved value for VDP R#26 register.
static volatile __at (0xfffc) uint8_t RG27SA; ///< `MSX2+` Saved value for VDP R#27 register.

#endif
