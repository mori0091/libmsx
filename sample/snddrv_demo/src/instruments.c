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
 * \file instruments.c
 */

#include <snd_i_table.h>

#define ARRAY_SIZEOF(a)    (sizeof(a) / sizeof(a[0]))

// Instrument #1
static const uint8_t i_table_01_a[] = {
  0x1e, 0x1c, 0x1a, 0x18, 0x16, 0x14, 0x12,
  0xff,
};
static const uint8_t i_table_01_s[] = {
  0x10,
  0xff,
};
static const uint8_t i_table_01_r[] = {
  0x0e, 0x0c, 0x0a, 0x08, 0x06, 0x04, 0x02, 0x00,
  0xff,
};
static const struct snd_i_table i_table_01 = {
// wait cnt, attack-decay, sustain     , release
  .wait = 1, i_table_01_a, i_table_01_s, i_table_01_r,
};

// Instrument #2
static const uint8_t i_table_02_a[] = {
  0x1e, 0x1c, 0x1a, 0x18, 0x16, 0x14, 0x12, 0x10,
  0x0e, 0x0c, 0x0a, 0x08, 0x06, 0x04, 0x02, 0x00,
};
static const uint8_t i_table_02_s[] = {
  0xff,
};
static const uint8_t i_table_02_r[] = {
  0xff,
};
static const struct snd_i_table i_table_02 = {
// wait cnt, attack-decay, sustain     , release
  .wait = 4, i_table_02_a, i_table_02_s, i_table_02_r,
};

// List of pointers to instruments.
const struct snd_i_table * i_tables[] = {
  // pointer to instrument #1
  &i_table_01,
  // pointer to instrument #2
  &i_table_02,
};
const uint8_t i_tables_length = ARRAY_SIZEOF(i_tables);
