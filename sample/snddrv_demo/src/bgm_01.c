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
  0x82,                                0,           // PSG chB:  OFF .. .... .... .... ....
  0x80,                                0,     0x09, // PSG chA:  OFF .. .... .... .... .... 0a

  0x82, 0x91, 0x8c, 0x61, 0x00,       36,  2,       // PSG chC:  C_2 02 vC.. f100 .... ....
  0x80,                               50,  1, 0x09, // PSG chA:  D_3 01 .... .... .... .... 0a
  0x82,                                0,           // PSG chB:  OFF .. .... .... .... ....
  0x80,                                0,     0x09, // PSG chA:  OFF .. .... .... .... .... 0a
  0x82, 0x91, 0x8f, 0x62, 0x00,       59,  2,       // PSG chC:  B_3 02 vF.. f200 .... ....
  0x80,                               50,  1, 0x09, // PSG chA:  D_3 01 .... .... .... .... 0a
  0x82,                                0,           // PSG chB:  OFF .. .... .... .... ....
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
  0x82,                                0,           // PSG chB:  OFF .. .... .... .... ....
  0x80,                                0,     0x09, // PSG chA:  OFF .. .... .... .... .... 0a

  0x82, 0x91, 0x8c, 0x61, 0x00,       36,  2,       // PSG chC:  C_2 02 vC.. f100 .... ....
  0x80,                               50,  1, 0x09, // PSG chA:  D_3 01 .... .... .... .... 0a
  0x82,                                0,           // PSG chB:  OFF .. .... .... .... ....
  0x80,                                0,     0x09, // PSG chA:  OFF .. .... .... .... .... 0a
  0x82, 0x91, 0x8f, 0x62, 0x00,       59,  2,       // PSG chC:  B_3 02 vF.. f200 .... ....
  0x80,                               50,  1, 0x09, // PSG chA:  D_3 01 .... .... .... .... 0a
  0x82,                                0,           // PSG chB:  OFF .. .... .... .... ....
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

const snd_Sound bgm_01 = {
  .tag = SND_STREAM,
  .replayRate = 60,             // 60Hz
  .stream = stream,
};
