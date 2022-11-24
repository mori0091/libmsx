// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021 Daishi Mori (mori0091)
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

static volatile __at (0xf3db) uint8_t CLIKSW; ///< `MSX` Key click beep switch (0:off, non-zero:on).

static volatile __at (0xf3df) uint8_t RG0SAV; ///< `MSX` Saved value for VDP R#0 register.
static volatile __at (0xf3e0) uint8_t RG1SAV; ///< `MSX` Saved value for VDP R#1 register.
static volatile __at (0xf3e1) uint8_t RG2SAV; ///< `MSX` Saved value for VDP R#2 register.
static volatile __at (0xf3e2) uint8_t RG3SAV; ///< `MSX` Saved value for VDP R#3 register.
static volatile __at (0xf3e3) uint8_t RG4SAV; ///< `MSX` Saved value for VDP R#4 register.
static volatile __at (0xf3e4) uint8_t RG5SAV; ///< `MSX` Saved value for VDP R#5 register.
static volatile __at (0xf3e5) uint8_t RG6SAV; ///< `MSX` Saved value for VDP R#6 register.
static volatile __at (0xf3e6) uint8_t RG7SAV; ///< `MSX` Saved value for VDP R#7 register.

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

static volatile __at (0xfffa) uint8_t RG25SA; ///< `MSX2+` Saved value for VDP R#25 register.
static volatile __at (0xfffb) uint8_t RG26SA; ///< `MSX2+` Saved value for VDP R#26 register.
static volatile __at (0xfffc) uint8_t RG27SA; ///< `MSX2+` Saved value for VDP R#27 register.

// ---- Slot workarea ----
/**
 * `MSX2` The slot address of SUB ROM
 *
 * \note In case of `MSX1`, there is no SUB ROM and `EXBRSA == 0`.
 */
static volatile __at (0xfaf8) uint8_t EXBRSA;

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

// ---- FDC workarea ----

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

#endif
