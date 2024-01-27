// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2024 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file audio_cb_scc_mode.c
 */

#include "audio__internal.h"

#include <scc.h>

void audio_cb_scc_mode(uint8_t mode) {
  SCC_set_mode(&audio_.scc, mode);
  SCC_enable(&audio_.scc);
}
