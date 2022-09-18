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
 * \file bgm_01.c
 */

#include <snd_sound.h>
#include <stdint.h>

// -------------------------------------------------------------
#if 0
// ---- Background music ----
static const uint8_t stream[] = {
  // A part (1/2)
  //ch, expressions...,             note, i#, wait, // channel: note i# expressions         wait+1
  0x82, 0x91, 0x8c, 0x61, 0x00,       36,  2,       // PSG chC:  C_2 02 vC.. f100 .... ....
  0x81, 0x91, 0x8c, 0x2c, 0x0c,       67,  1,       // PSG chB:  G_4 01 vC.. .... cC0C ....
  0x80, 0x91, 0x8f, 0x15, 0x70,       60,  1, 0x04, // PSG chA:  C_4 01 vF.. .... b57. .... 05
  0x81,                               69,  1, 0x04, // PSG chB:  A_4 01 .... .... .... .... 05
  0x82,                                0,           // PSG chC:  OFF .. .... .... .... ....
  0x81,                               67,  1,       // PSG chB:  G_4 01 .... .... .... ....
  0x80,                                0,     0x04, // PSG chA:  OFF .. .... .... .... .... 05
  0x81,                               69,  1, 0x04, // PSG chB:  A_4 01 .... .... .... .... 05
  0x82, 0x91, 0x8f, 0x62, 0x00,       59,  2,       // PSG chC:  B_3 02 vF.. f200 .... ....
  0x81,                               67,  1,       // PSG chB:  G_4 01 .... .... .... ....
  0x80,                               60,  1, 0x04, // PSG chA:  C_4 01 .... .... .... .... 05
  0x81,                               69,  1, 0x04, // PSG chB:  A_4 01 .... .... .... .... 05
  0x82,                                0,           // PSG chC:  OFF .. .... .... .... ....
  0x81,                               67,  1,       // PSG chB:  G_4 01 .... .... .... ....
  0x80,                                0,     0x04, // PSG chA:  OFF .. .... .... .... .... 05
  0x81,                               69,  1, 0x04, // PSG chB:  A_4 01 .... .... .... .... 05

  0x82, 0x91, 0x8d, 0x63, 0x00,       36,  2,       // PSG chC:  C_2 02 vD.. f300 .... ....
  0x81, 0x90, 0x8d,                   67,  2,       // PSG chB:  G_4 02 vD.. .... .... ....
  0x80, 0x90, 0x70, 0x80,             62,  1, 0x04, // PSG chA:  D_4 01 .... g080 .... .... 05
  0x81,                               69,  2, 0x04, // PSG chB:  A_4 02 .... .... .... .... 05
  0x82, 0x90,       0x64, 0x00,       36,  2,       // PSG chC:  C_2 02 .... f400 .... ....
  0x81,                               67,  2,       // PSG chB:  G_4 02 .... .... .... ....
  0x80,                                0,     0x04, // PSG chA:  OFF .. .... .... .... .... 05
  0x81,                               69,  2, 0x04, // PSG chB:  A_4 02 .... .... .... .... 05
  0x82, 0x90,       0x65, 0x00,       59,  2,       // PSG chC:  B_3 02 .... f500 .... ....
  0x81,                               67,  1,       // PSG chB:  G_4 01 .... .... .... ....
  0x80,                               62,  1, 0x04, // PSG chA:  D_4 01 .... .... .... .... 05
  0x81,                               69,  1, 0x04, // PSG chB:  A_4 01 .... .... .... .... 05
  0x82,                                0,           // PSG chC:  OFF .. .... .... .... ....
  0x81,                               67,  1,       // PSG chB:  G_4 01 .... .... .... ....
  0x80,                                0,     0x04, // PSG chA:  OFF .. .... .... .... .... 05
  0x81,                               69,  1, 0x04, // PSG chB:  A_4 01 .... .... .... .... 05

  0x82, 0x91, 0x8c, 0x61, 0x00,       36,  2,       // PSG chC:  C_2 02 vC.. f100 .... ....
  0x81, 0x90, 0x8e,                   67,  1,       // PSG chB:  G_4 01 vE.. .... .... ....
  0x80, 0x90, 0x70, 0x80,             60,  1, 0x04, // PSG chA:  C_4 01 .... g080 .... .... 05
  0x81,                               69,  1, 0x04, // PSG chB:  A_4 01 .... .... .... .... 05
  0x82,                                0,           // PSG chC:  OFF .. .... .... .... ....
  0x81,                               67,  1,       // PSG chB:  G_4 01 .... .... .... ....
  0x80,                                0,     0x04, // PSG chA:  OFF .. .... .... .... .... 05
  0x81,                               69,  1, 0x04, // PSG chB:  A_4 01 .... .... .... .... 05
  0x82, 0x91, 0x8f, 0x62, 0x00,       59,  2,       // PSG chC:  B_3 02 vF.. f200 .... ....
  0x81,                               67,  2,       // PSG chB:  G_4 02 .... .... .... ....
  0x80,                               60,  1, 0x04, // PSG chA:  C_4 01 .... .... .... .... 05
  0x81,                               69,  2, 0x04, // PSG chB:  A_4 01 .... .... .... .... 05
  0x82,                                0,           // PSG chC:  OFF .. .... .... .... ....
  0x81,                               67,  2,       // PSG chB:  G_4 02 .... .... .... ....
  0x80,                                0,     0x04, // PSG chA:  OFF .. .... .... .... .... 05
  0x81,                               69,  2, 0x04, // PSG chB:  A_4 01 .... .... .... .... 05

  0x82, 0x91, 0x8d, 0x63, 0x00,       36,  2,       // PSG chC:  C_2 02 vD.. f300 .... ....
  0x81, 0x90, 0x8f,                   67,  1,       // PSG chB:  G_4 01 vF.. .... .... ....
  0x80,                               55,  1, 0x04, // PSG chA:  G_3 01 .... .... .... .... 05
  0x81,                               69,  2, 0x04, // PSG chB:  A_4 01 .... .... .... .... 05
  0x82, 0x90,       0x64, 0x00,       36,  2,       // PSG chC:  C_2 02 .... f400 .... ....
  0x81,                               67,  1,       // PSG chB:  G_4 01 .... .... .... ....
  0x80, 0x80, 0xa0, 0x80,                     0x04, // PSG chA:  ... .. o080 .... .... .... 05
  0x81,                               69,  1,       // PSG chB:  A_4 01 .... .... .... ....
  0x80, 0x80, 0x90, 0x80,                     0x01, // PSG chA:  ... .. i080 .... .... .... 02
  0x80, 0x80, 0xa0, 0x80,                     0x01, // PSG chA:  ... .. o080 .... .... .... 02
  0x80, 0x80, 0x90, 0x80,                     0x00, // PSG chA:  ... .. i080 .... .... .... 01

  0x82, 0x90,       0x65, 0x00,       71,  2,       // PSG chC:  B_4 02 .... f500 .... ....
  0x81,                               76,  1,       // PSG chB:  E_5 01 .... .... .... ....
  0x80, 0x80, 0xa0, 0x80,                     0x09, // PSG chA:  ... .. o080 .... .... .... 0a
  0x81, 0x81, 0xa0, 0x08, 0x20, 0x0c,         0x09, // PSG chB:  ... .. o008 .... c00C .... 0a

  // A part (2/2)
  //ch, expressions...,             note, i#, wait, // channel: note i# expressions         wait+1
  0x82, 0x91, 0x8c, 0x61, 0x00,       36,  2,       // PSG chC:  C_2 02 vC.. f100 .... ....
  0x81, 0x91, 0x8c, 0x2c, 0x0c,       67,  1,       // PSG chB:  G_4 01 vC.. .... cC0C ....
  0x80, 0x91, 0x8f, 0x15, 0x70,       60,  1, 0x04, // PSG chA:  C_4 01 vF.. .... b57. .... 05
  0x81,                               69,  1, 0x04, // PSG chB:  A_4 01 .... .... .... .... 05
  0x82,                                0,           // PSG chC:  OFF .. .... .... .... ....
  0x81,                               67,  1,       // PSG chB:  G_4 01 .... .... .... ....
  0x80,                                0,     0x04, // PSG chA:  OFF .. .... .... .... .... 05
  0x81,                               69,  1, 0x04, // PSG chB:  A_4 01 .... .... .... .... 05
  0x82, 0x91, 0x8f, 0x62, 0x00,       59,  2,       // PSG chC:  B_3 02 vF.. f200 .... ....
  0x81,                               67,  1,       // PSG chB:  G_4 01 .... .... .... ....
  0x80,                               60,  1, 0x04, // PSG chA:  C_4 01 .... .... .... .... 05
  0x81,                               69,  1, 0x04, // PSG chB:  A_4 01 .... .... .... .... 05
  0x82,                                0,           // PSG chC:  OFF .. .... .... .... ....
  0x81,                               67,  1,       // PSG chB:  G_4 01 .... .... .... ....
  0x80,                                0,     0x04, // PSG chA:  OFF .. .... .... .... .... 05
  0x81,                               69,  1, 0x04, // PSG chB:  A_4 01 .... .... .... .... 05

  0x82, 0x91, 0x8d, 0x63, 0x00,       36,  2,       // PSG chC:  C_2 02 vD.. f300 .... ....
  0x81, 0x90, 0x8d,                   67,  2,       // PSG chB:  G_4 02 vD.. .... .... ....
  0x80, 0x90, 0x70, 0x80,             62,  1, 0x04, // PSG chA:  D_4 01 .... g080 .... .... 05
  0x81,                               69,  2, 0x04, // PSG chB:  A_4 02 .... .... .... .... 05
  0x82, 0x90,       0x64, 0x00,       36,  2,       // PSG chC:  C_2 02 .... f400 .... ....
  0x81,                               67,  2,       // PSG chB:  G_4 02 .... .... .... ....
  0x80,                                0,     0x04, // PSG chA:  OFF .. .... .... .... .... 05
  0x81,                               69,  2, 0x04, // PSG chB:  A_4 02 .... .... .... .... 05
  0x82, 0x90,       0x65, 0x00,       59,  2,       // PSG chC:  B_3 02 .... f500 .... ....
  0x81,                               67,  1,       // PSG chB:  G_4 01 .... .... .... ....
  0x80,                               62,  1, 0x04, // PSG chA:  D_4 01 .... .... .... .... 05
  0x81,                               69,  1, 0x04, // PSG chB:  A_4 01 .... .... .... .... 05
  0x82,                                0,           // PSG chC:  OFF .. .... .... .... ....
  0x81,                               67,  1,       // PSG chB:  G_4 01 .... .... .... ....
  0x80,                                0,     0x04, // PSG chA:  OFF .. .... .... .... .... 05
  0x81,                               69,  1, 0x04, // PSG chB:  A_4 01 .... .... .... .... 05

  0x82, 0x91, 0x8c, 0x61, 0x00,       36,  2,       // PSG chC:  C_2 02 vC.. f100 .... ....
  0x81, 0x90, 0x8e,                   67,  1,       // PSG chB:  G_4 01 vE.. .... .... ....
  0x80, 0x90, 0x70, 0x80,             60,  1, 0x04, // PSG chA:  C_4 01 .... g080 .... .... 05
  0x81,                               69,  1, 0x04, // PSG chB:  A_4 01 .... .... .... .... 05
  0x82,                                0,           // PSG chC:  OFF .. .... .... .... ....
  0x81,                               67,  1,       // PSG chB:  G_4 01 .... .... .... ....
  0x80,                                0,     0x04, // PSG chA:  OFF .. .... .... .... .... 05
  0x81,                               69,  1, 0x04, // PSG chB:  A_4 01 .... .... .... .... 05
  0x82, 0x91, 0x8f, 0x62, 0x00,       59,  2,       // PSG chC:  B_3 02 vF.. f200 .... ....
  0x81,                               67,  2,       // PSG chB:  G_4 02 .... .... .... ....
  0x80,                               60,  1, 0x04, // PSG chA:  C_4 01 .... .... .... .... 05
  0x81,                               69,  2, 0x04, // PSG chB:  A_4 01 .... .... .... .... 05
  0x82,                                0,           // PSG chC:  OFF .. .... .... .... ....
  0x81,                               67,  2,       // PSG chB:  G_4 02 .... .... .... ....
  0x80,                                0,     0x04, // PSG chA:  OFF .. .... .... .... .... 05
  0x81,                               69,  2, 0x04, // PSG chB:  A_4 01 .... .... .... .... 05

  0x82, 0x91, 0x8d, 0x63, 0x00,       36,  2,       // PSG chC:  C_2 02 vD.. f300 .... ....
  0x81, 0x90, 0x8f,                   67,  1,       // PSG chB:  G_4 01 vF.. .... .... ....
  0x80,                               55,  1, 0x04, // PSG chA:  G_3 01 .... .... .... .... 05
  0x81,                               69,  2, 0x04, // PSG chB:  A_4 01 .... .... .... .... 05
  0x82, 0x90,       0x64, 0x00,       36,  2,       // PSG chC:  C_2 02 .... f400 .... ....
  0x81,                               67,  1,       // PSG chB:  G_4 01 .... .... .... ....
  0x80, 0x80, 0xa0, 0x80,                     0x04, // PSG chA:  ... .. o080 .... .... .... 05
  0x81,                               69,  1,       // PSG chB:  A_4 01 .... .... .... ....
  0x80, 0x80, 0x90, 0x80,                     0x01, // PSG chA:  ... .. i080 .... .... .... 02
  0x80, 0x80, 0xa0, 0x80,                     0x01, // PSG chA:  ... .. o080 .... .... .... 02
  0x80, 0x80, 0x90, 0x80,                     0x00, // PSG chA:  ... .. i080 .... .... .... 01

  0x82, 0x90,       0x65, 0x00,       71,  2,       // PSG chC:  B_4 02 .... f500 .... ....
  0x81,                               76,  1,       // PSG chB:  E_5 01 .... .... .... ....
  0x80, 0x80, 0xa0, 0x80,                     0x09, // PSG chA:  ... .. o080 .... .... .... 0a
  0x81, 0x81, 0xa0, 0x08, 0x20, 0x0c,         0x09, // PSG chB:  ... .. o008 .... c00C .... 0a

  // B part (1/2)
  //ch, expressions...,             note, i#, wait, // channel: note i# expressions         wait+1
  0x82, 0x91, 0x8c, 0x61, 0x00,       36,  2,       // PSG chC:  C_2 02 vC.. f100 .... ....
  0x80, 0x91, 0x8c, 0x71, 0x00,       50,  1, 0x09, // PSG chA:  D_3 01 vC.. g100 .... .... 0a
  0x82,                                0,           // PSG chC:  OFF .. .... .... .... ....
  0x80,                                0,     0x09, // PSG chA:  OFF .. .... .... .... .... 0a
  0x82, 0x91, 0x8f, 0x62, 0x00,       59,  2,       // PSG chC:  B_3 02 vF.. f200 .... ....
  0x80,                               50,  1, 0x09, // PSG chA:  D_3 01 .... .... .... .... 0a
  0x82,                                0,           // PSG chC:  OFF .. .... .... .... ....
  0x80,                                0,     0x09, // PSG chA:  OFF .. .... .... .... .... 0a

  0x82, 0x91, 0x8d, 0x63, 0x00,       36,  2,       // PSG chC:  C_2 02 vD.. f300 .... ....
  0x80,                               48,  1, 0x09, // PSG chA:  C_3 01 .... .... .... .... 0a
  0x82, 0x90,       0x64, 0x00,       36,  2,       // PSG chC:  C_2 02 .... f400 .... ....
  0x80,                                0,     0x09, // PSG chA:  OFF .. .... .... .... .... 0a
  0x82, 0x90,       0x65, 0x00,       59,  2,       // PSG chC:  B_3 02 .... f500 .... ....
  0x80,                               48,  1, 0x09, // PSG chA:  C_3 01 .... .... .... .... 0a
  0x82,                                0,           // PSG chC:  OFF .. .... .... .... ....
  0x80,                                0,     0x09, // PSG chA:  OFF .. .... .... .... .... 0a

  0x82, 0x91, 0x8c, 0x61, 0x00,       36,  2,       // PSG chC:  C_2 02 vC.. f100 .... ....
  0x80,                               50,  1, 0x09, // PSG chA:  D_3 01 .... .... .... .... 0a
  0x82,                                0,           // PSG chC:  OFF .. .... .... .... ....
  0x80,                                0,     0x09, // PSG chA:  OFF .. .... .... .... .... 0a
  0x82, 0x91, 0x8f, 0x62, 0x00,       59,  2,       // PSG chC:  B_3 02 vF.. f200 .... ....
  0x80,                               50,  1, 0x09, // PSG chA:  D_3 01 .... .... .... .... 0a
  0x82,                                0,           // PSG chC:  OFF .. .... .... .... ....
  0x80,                                0,     0x09, // PSG chA:  OFF .. .... .... .... .... 0a

  0x82, 0x91, 0x8d, 0x63, 0x00,       36,  2,       // PSG chC:  C_2 02 vD.. f300 .... ....
  0x80,                               55,  1, 0x09, // PSG chA:  G_3 01 .... .... .... .... 0a
  0x82, 0x90,       0x64, 0x00,       36,  2,       // PSG chC:  C_2 02 .... f400 .... ....
  0x80, 0x80, 0xa0, 0x80,                     0x04, // PSG chA:  ... .. o080 .... .... .... 05
  0x80, 0x80, 0x90, 0x80,                     0x01, // PSG chA:  ... .. i080 .... .... .... 02
  0x80, 0x80, 0xa0, 0x80,                     0x01, // PSG chA:  ... .. o080 .... .... .... 02
  0x80, 0x80, 0x90, 0x80,                     0x00, // PSG chA:  ... .. i080 .... .... .... 01

  0x82, 0x90,       0x65, 0x00,       71,  2,       // PSG chC:  B_4 02 .... f500 .... ....
                                              0x13, //           ... .. .... .... .... .... 14

  // B part (2/2)
  //ch, expressions...,             note, i#, wait, // channel: note i# expressions         wait+1
  0x82, 0x91, 0x8c, 0x61, 0x00,       36,  2,       // PSG chC:  C_2 02 vC.. f100 .... ....
  0x80, 0x91, 0x8c, 0x71, 0x00,       50,  1, 0x09, // PSG chA:  D_3 01 vC.. g100 .... .... 0a
  0x82,                                0,           // PSG chC:  OFF .. .... .... .... ....
  0x80,                                0,     0x09, // PSG chA:  OFF .. .... .... .... .... 0a
  0x82, 0x91, 0x8f, 0x62, 0x00,       59,  2,       // PSG chC:  B_3 02 vF.. f200 .... ....
  0x80,                               50,  1, 0x09, // PSG chA:  D_3 01 .... .... .... .... 0a
  0x82,                                0,           // PSG chC:  OFF .. .... .... .... ....
  0x80,                                0,     0x09, // PSG chA:  OFF .. .... .... .... .... 0a

  0x82, 0x91, 0x8d, 0x63, 0x00,       36,  2,       // PSG chC:  C_2 02 vD.. f300 .... ....
  0x80,                               48,  1, 0x09, // PSG chA:  C_3 01 .... .... .... .... 0a
  0x82, 0x90,       0x64, 0x00,       36,  2,       // PSG chC:  C_2 02 .... f400 .... ....
  0x80,                                0,     0x09, // PSG chA:  OFF .. .... .... .... .... 0a
  0x82, 0x90,       0x65, 0x00,       59,  2,       // PSG chC:  B_3 02 .... f500 .... ....
  0x80,                               48,  1, 0x09, // PSG chA:  C_3 01 .... .... .... .... 0a
  0x82,                                0,           // PSG chC:  OFF .. .... .... .... ....
  0x80,                                0,     0x09, // PSG chA:  OFF .. .... .... .... .... 0a

  0x82, 0x91, 0x8c, 0x61, 0x00,       36,  2,       // PSG chC:  C_2 02 vC.. f100 .... ....
  0x80,                               50,  1, 0x09, // PSG chA:  D_3 01 .... .... .... .... 0a
  0x82,                                0,           // PSG chC:  OFF .. .... .... .... ....
  0x80,                                0,     0x09, // PSG chA:  OFF .. .... .... .... .... 0a
  0x82, 0x91, 0x8f, 0x62, 0x00,       59,  2,       // PSG chC:  B_3 02 vF.. f200 .... ....
  0x80,                               50,  1, 0x09, // PSG chA:  D_3 01 .... .... .... .... 0a
  0x82,                                0,           // PSG chC:  OFF .. .... .... .... ....
  0x80,                                0,     0x09, // PSG chA:  OFF .. .... .... .... .... 0a

  0x82, 0x91, 0x8d, 0x63, 0x00,       36,  2,       // PSG chC:  C_2 02 vD.. f300 .... ....
  0x80,                               55,  1, 0x09, // PSG chA:  G_3 01 .... .... .... .... 0a
  0x82, 0x90,       0x64, 0x00,       36,  2,       // PSG chC:  C_2 02 .... f400 .... ....
  0x80, 0x80, 0xa0, 0x80,                     0x04, // PSG chA:  ... .. o080 .... .... .... 05
  0x80, 0x80, 0x90, 0x80,                     0x01, // PSG chA:  ... .. i080 .... .... .... 02
  0x80, 0x80, 0xa0, 0x80,                     0x01, // PSG chA:  ... .. o080 .... .... .... 02
  0x80, 0x80, 0x90, 0x80,                     0x00, // PSG chA:  ... .. i080 .... .... .... 01

  0x82, 0x90,       0x65, 0x00,       71,  2,       // PSG chC:  B_4 02 .... f500 .... ....
                                              0x13, //           ... .. .... .... .... .... 14

  0xff,                                               // EOM
};

const snd_Stream bgm_01 = {
  .replayRate = 60,             // 60Hz
  .stream = stream,
};

// -------------------------------------------------------------
#else

// ---- track #1 -----------------------------------------------
static const uint8_t track1[] = {
  //expressions...,           note, i#, wait, // note i# expressions         wait+1
  0x91, 0x8f, 0x15, 0x70,       60,  1, 0xc4, // PSG chA:  C_4 01 vF.. .... b57. .... 05
                                        0xc4, // PSG chA:  ... .. .... .... .... .... 05
                                 0,     0xc4, // PSG chA:  OFF .. .... .... .... .... 05
                                        0xc4, // PSG chA:  ... .. .... .... .... .... 05
                                60,  1, 0xc4, // PSG chA:  C_4 01 .... .... .... .... 05
                                        0xc4, // PSG chA:  ... .. .... .... .... .... 05
                                 0,     0xc4, // PSG chA:  OFF .. .... .... .... .... 05
                                        0xc4, // PSG chA:  ... .. .... .... .... .... 05
  0x90, 0x70, 0x80,             62,  1, 0xc4, // PSG chA:  D_4 01 .... g080 .... .... 05
                                        0xc4, // PSG chA:  ... .. .... .... .... .... 05
                                 0,     0xc4, // PSG chA:  OFF .. .... .... .... .... 05
                                        0xc4, // PSG chA:  ... .. .... .... .... .... 05
                                62,  1, 0xc4, // PSG chA:  D_4 01 .... .... .... .... 05
                                        0xc4, // PSG chA:  ... .. .... .... .... .... 05
                                 0,     0xc4, // PSG chA:  OFF .. .... .... .... .... 05
                                        0xc4, // PSG chA:  ... .. .... .... .... .... 05
  0x90, 0x70, 0x80,             60,  1, 0xc4, // PSG chA:  C_4 01 .... g080 .... .... 05
                                        0xc4, // PSG chA:  ... .. .... .... .... .... 05
                                 0,     0xc4, // PSG chA:  OFF .. .... .... .... .... 05
                                        0xc4, // PSG chA:  ... .. .... .... .... .... 05
                                60,  1, 0xc4, // PSG chA:  C_4 01 .... .... .... .... 05
                                        0xc4, // PSG chA:  ... .. .... .... .... .... 05
                                 0,     0xc4, // PSG chA:  OFF .. .... .... .... .... 05
                                        0xc4, // PSG chA:  ... .. .... .... .... .... 05
                                55,  1, 0xc4, // PSG chA:  G_3 01 .... .... .... .... 05
                                        0xc4, // PSG chA:  ... .. .... .... .... .... 05
  0x80, 0xa0, 0x80,                     0xc4, // PSG chA:  ... .. o080 .... .... .... 05
  0x80, 0x90, 0x80,                     0xc1, // PSG chA:  ... .. i080 .... .... .... 02
  0x80, 0xa0, 0x80,                     0xc1, // PSG chA:  ... .. o080 .... .... .... 02
  0x80, 0x90, 0x80,                     0xc0, // PSG chA:  ... .. i080 .... .... .... 01
  0x80, 0xa0, 0x80,                     0xc9, // PSG chA:  ... .. o080 .... .... .... 0a
                                        0xc9, // PSG chA:  ... .. .... .... .... .... 0a
};
// ---- track #2 -----------------------------------------------
static const uint8_t track2[] = {
  //expressions...,           note, i#, wait, //          note i# expressions         wait+1
  0x91, 0x8c, 0x2c, 0x0c,       67,  1, 0xc4, // PSG chB:  G_4 01 vC.. .... cC0C .... 05
                                69,  1, 0xc4, // PSG chB:  A_4 01 .... .... .... .... 05
                                67,  1, 0xc4, // PSG chB:  G_4 01 .... .... .... .... 05
                                69,  1, 0xc4, // PSG chB:  A_4 01 .... .... .... .... 05
                                67,  1, 0xc4, // PSG chB:  G_4 01 .... .... .... .... 05
                                69,  1, 0xc4, // PSG chB:  A_4 01 .... .... .... .... 05
                                67,  1, 0xc4, // PSG chB:  G_4 01 .... .... .... .... 05
                                69,  1, 0xc4, // PSG chB:  A_4 01 .... .... .... .... 05
  0x90, 0x8d,                   67,  2, 0xc4, // PSG chB:  G_4 02 vD.. .... .... .... 05
                                69,  2, 0xc4, // PSG chB:  A_4 02 .... .... .... .... 05
                                67,  2, 0xc4, // PSG chB:  G_4 02 .... .... .... .... 05
                                69,  2, 0xc4, // PSG chB:  A_4 02 .... .... .... .... 05
                                67,  1, 0xc4, // PSG chB:  G_4 01 .... .... .... .... 05
                                69,  1, 0xc4, // PSG chB:  A_4 01 .... .... .... .... 05
                                67,  1, 0xc4, // PSG chB:  G_4 01 .... .... .... .... 05
                                69,  1, 0xc4, // PSG chB:  A_4 01 .... .... .... .... 05
  0x90, 0x8e,                   67,  1, 0xc4, // PSG chB:  G_4 01 vE.. .... .... .... 05
                                69,  1, 0xc4, // PSG chB:  A_4 01 .... .... .... .... 05
                                67,  1, 0xc4, // PSG chB:  G_4 01 .... .... .... .... 05
                                69,  1, 0xc4, // PSG chB:  A_4 01 .... .... .... .... 05
                                67,  2, 0xc4, // PSG chB:  G_4 02 .... .... .... .... 05
                                69,  2, 0xc4, // PSG chB:  A_4 01 .... .... .... .... 05
                                67,  2, 0xc4, // PSG chB:  G_4 02 .... .... .... .... 05
                                69,  2, 0xc4, // PSG chB:  A_4 01 .... .... .... .... 05
  0x90, 0x8f,                   67,  1, 0xc4, // PSG chB:  G_4 01 vF.. .... .... .... 05
                                69,  2, 0xc4, // PSG chB:  A_4 01 .... .... .... .... 05
                                67,  1, 0xc4, // PSG chB:  G_4 01 .... .... .... .... 05
                                69,  1, 0xc1, // PSG chB:  A_4 01 .... .... .... .... 02
                                        0xc1, // PSG chB:  ... .. .... .... .... .... 02
                                        0xc0, // PSG chB:  ... .. .... .... .... .... 01
                                76,  1, 0xc9, // PSG chB:  E_5 01 .... .... .... .... 0a
  0x81, 0xa0, 0x08, 0x20, 0x0c,         0xc9, // PSG chB:  ... .. o008 .... c00C .... 0a
};
// ---- track #3 -----------------------------------------------
static const uint8_t track3[] = {
  //expressions...,           note, i#, wait, //          note i# expressions         wait+1
  0x91, 0x8c, 0x61, 0x00,       36,  2, 0xc4, // PSG chC:  C_2 02 vC.. f100 .... .... 05
                                        0xc4, // PSG chC:  ... .. .... .... .... .... 05
                                 0,     0xc4, // PSG chC:  OFF .. .... .... .... .... 05
                                        0xc4, // PSG chC:  ... .. .... .... .... .... 05
  0x91, 0x8f, 0x62, 0x00,       59,  2, 0xc4, // PSG chC:  B_3 02 vF.. f200 .... .... 05
                                        0xc4, // PSG chC:  ... .. .... .... .... .... 05
                                 0,     0xc4, // PSG chC:  OFF .. .... .... .... .... 05
                                        0xc4, // PSG chC:  ... .. .... .... .... .... 05
  0x91, 0x8d, 0x63, 0x00,       36,  2, 0xc4, // PSG chC:  C_2 02 vD.. f300 .... .... 05
                                        0xc4, // PSG chC:  ... .. .... .... .... .... 05
  0x90,       0x64, 0x00,       36,  2, 0xc4, // PSG chC:  C_2 02 .... f400 .... .... 05
                                        0xc4, // PSG chC:  ... .. .... .... .... .... 05
  0x90,       0x65, 0x00,       59,  2, 0xc4, // PSG chC:  B_3 02 .... f500 .... .... 05
                                        0xc4, // PSG chC:  ... .. .... .... .... .... 05
                                 0,     0xc4, // PSG chC:  OFF .. .... .... .... .... 05
                                        0xc4, // PSG chC:  ... .. .... .... .... .... 05
  0x91, 0x8c, 0x61, 0x00,       36,  2, 0xc4, // PSG chC:  C_2 02 vC.. f100 .... .... 05
                                        0xc4, // PSG chC:  ... .. .... .... .... .... 05
                                 0,     0xc4, // PSG chC:  OFF .. .... .... .... .... 05
                                        0xc4, // PSG chC:  ... .. .... .... .... .... 05
  0x91, 0x8f, 0x62, 0x00,       59,  2, 0xc4, // PSG chC:  B_3 02 vF.. f200 .... .... 05
                                        0xc4, // PSG chC:  ... .. .... .... .... .... 05
                                 0,     0xc4, // PSG chC:  OFF .. .... .... .... .... 05
                                        0xc4, // PSG chC:  ... .. .... .... .... .... 05
  0x91, 0x8d, 0x63, 0x00,       36,  2, 0xc4, // PSG chC:  C_2 02 vD.. f300 .... .... 05
                                        0xc4, // PSG chC:  ... .. .... .... .... .... 05
  0x90,       0x64, 0x00,       36,  2, 0xc4, // PSG chC:  C_2 02 .... f400 .... .... 05
                                        0xc1, // PSG chC:  ... .. .... .... .... .... 02
                                        0xc1, // PSG chC:  ... .. .... .... .... .... 02
                                        0xc0, // PSG chC:  ... .. .... .... .... .... 01
  0x90,       0x65, 0x00,       71,  2, 0xc9, // PSG chC:  B_4 02 .... f500 .... .... 0a
                                        0xc9, // PSG chC:  ... .. .... .... .... .... 0a
};
// ---- track #4 -----------------------------------------------
static const uint8_t track4[] = {
  //expressions...,           note, i#, wait, // note i# expressions         wait+1
  0x91, 0x8c, 0x71, 0x00,       50,  1, 0xc9, // PSG chA:  D_3 01 vC.. g100 .... .... 0a
                                 0,     0xc9, // PSG chA:  OFF .. .... .... .... .... 0a
                                50,  1, 0xc9, // PSG chA:  D_3 01 .... .... .... .... 0a
                                 0,     0xc9, // PSG chA:  OFF .. .... .... .... .... 0a
                                48,  1, 0xc9, // PSG chA:  C_3 01 .... .... .... .... 0a
                                 0,     0xc9, // PSG chA:  OFF .. .... .... .... .... 0a
                                48,  1, 0xc9, // PSG chA:  C_3 01 .... .... .... .... 0a
                                 0,     0xc9, // PSG chA:  OFF .. .... .... .... .... 0a
                                50,  1, 0xc9, // PSG chA:  D_3 01 .... .... .... .... 0a
                                 0,     0xc9, // PSG chA:  OFF .. .... .... .... .... 0a
                                50,  1, 0xc9, // PSG chA:  D_3 01 .... .... .... .... 0a
                                 0,     0xc9, // PSG chA:  OFF .. .... .... .... .... 0a
                                55,  1, 0xc9, // PSG chA:  G_3 01 .... .... .... .... 0a
  0x80, 0xa0, 0x80,                     0xc4, // PSG chA:  ... .. o080 .... .... .... 05
  0x80, 0x90, 0x80,                     0xc1, // PSG chA:  ... .. i080 .... .... .... 02
  0x80, 0xa0, 0x80,                     0xc1, // PSG chA:  ... .. o080 .... .... .... 02
  0x80, 0x90, 0x80,                     0xc0, // PSG chA:  ... .. i080 .... .... .... 01
                                        0xd3, // PSG chA   ... .. .... .... .... .... 14
};
// ---- track #5 -----------------------------------------------
// static const uint8_t track5[] = {
// };
// ---- track #6 -----------------------------------------------
static const uint8_t track6[] = {
  //expressions...,           note, i#, wait, // note i# expressions         wait+1
  0x91, 0x8c, 0x61, 0x00,       36,  2, 0xc9, // PSG chC:  C_2 02 vC.. f100 .... .... 0a
                                 0,     0xc9, // PSG chC:  OFF .. .... .... .... .... 0a
  0x91, 0x8f, 0x62, 0x00,       59,  2, 0xc9, // PSG chC:  B_3 02 vF.. f200 .... .... 0a
                                 0,     0xc9, // PSG chC:  OFF .. .... .... .... .... 0a
  0x91, 0x8d, 0x63, 0x00,       36,  2, 0xc9, // PSG chC:  C_2 02 vD.. f300 .... .... 0a
  0x90,       0x64, 0x00,       36,  2, 0xc9, // PSG chC:  C_2 02 .... f400 .... .... 0a
  0x90,       0x65, 0x00,       59,  2, 0xc9, // PSG chC:  B_3 02 .... f500 .... .... 0a
                                 0,     0xc9, // PSG chC:  OFF .. .... .... .... .... 0a
  0x91, 0x8c, 0x61, 0x00,       36,  2, 0xc9, // PSG chC:  C_2 02 vC.. f100 .... .... 0a
                                 0,     0xc9, // PSG chC:  OFF .. .... .... .... .... 0a
  0x91, 0x8f, 0x62, 0x00,       59,  2, 0xc9, // PSG chC:  B_3 02 vF.. f200 .... .... 0a
                                 0,     0xc9, // PSG chC:  OFF .. .... .... .... .... 0a
  0x91, 0x8d, 0x63, 0x00,       36,  2, 0xc9, // PSG chC:  C_2 02 vD.. f300 .... .... 0a
  0x90,       0x64, 0x00,       36,  2, 0xc4, // PSG chC:  C_2 02 .... f400 .... .... 05
                                        0xc1, // PSG chC   ... .. .... .... .... .... 02
                                        0xc1, // PSG chC   ... .. .... .... .... .... 02
                                        0xc0, // PSG chC   ... .. .... .... .... .... 01
  0x90,       0x65, 0x00,       71,  2, 0xd3, // PSG chC:  B_4 02 .... f500 .... .... 14
};


#define ARRAY_SIZEOF(a)   (sizeof(a)/sizeof(a[0]))
#define VEC_FROM_ARRAY(a) {.length = ARRAY_SIZEOF(a), .data = a}

// ---- all tracks ---------------------------------------------
static const snd_Track tracks[] = {
  [0] = {0},
  [1] = VEC_FROM_ARRAY(track1),
  [2] = VEC_FROM_ARRAY(track2),
  [3] = VEC_FROM_ARRAY(track3),
  [4] = VEC_FROM_ARRAY(track4),
  [5] = {0},
  [6] = VEC_FROM_ARRAY(track6),
};

// ---- patterns -----------------------------------------------
static const snd_Pattern patterns[] = {
  {160, {0}, {{1}, {2}, {3}}},
  {160, {0}, {{1}, {2}, {3}}},
  {160, {0}, {{4}, {5}, {6}}},
  {160, {0}, {{4}, {5}, {6}}},
};

// ---- program ------------------------------------------------
const snd_Program bgm_01 = {
  .replayRate = 60,             // Hz
  .speedTracks = {0},           // no speed tracks
  .eventTracks = {0},           // no event tracks
  .tracks = VEC_FROM_ARRAY(tracks),
  .patterns = VEC_FROM_ARRAY(patterns),
  .initialSpeed = 0,            // ticks per line (L16 = 5, L4 = 20)
  .isLoop = false,
  .loopToIndex = 0,
};

#endif
