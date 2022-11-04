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
 * \file snd_set_i_tables.c
 *
 */

#include "../include/snd_i_table.h"

#include "./snd_i.h"

#define DI()  __asm__("di")
#define EI()  __asm__("ei")

extern void snd_stop(void);

void snd_set_i_tables(uint8_t n, const struct snd_i_table * i_tables) {
  snd_stop();
  DI();
  snd_i__set_i_tables(n, i_tables);
  EI();
}
