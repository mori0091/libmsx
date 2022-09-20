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
 * \file snd_default_i_tables.c
 *
 */

#include "./snd_i.h"

static const uint8_t AD[] = {
  0x1e, 0x1c, 0x1a, 0x18, 0x16, 0x14, 0x12, /* 0xff, */
};
static const uint8_t S[] = {
  0x10, 0xff,
};
static const uint8_t R[] = {
  0x0e, 0x0c, 0x0a, 0x08, 0x06, 0x04, 0x02, 0x00, 0xff,
};
static const struct snd_i_table i_table_01 = {
  .wait    = 1,
  .ad_part = AD,
  .s_part  = S,
  .r_part  = R,
};

const struct snd_i_table * snd_default_i_tables[] = {
  &i_table_01,
};
