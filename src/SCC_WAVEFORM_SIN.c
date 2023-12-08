// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2023 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file SCC_WAVEFORM_SIN.c
 */

#include "scc_wav.h"

const int8_t SCC_WAVEFORM_SIN[32] = {
     0,  +25,  +49,  +71,  +90, +106, +117, +125,
  +127, +125, +117, +106,  +90,  +71,  +49,  +25,
     0,  -25,  -49,  -71,  -90, -106, -117, -125,
  -127, -125, -117, -106,  -90,  -71,  -49,  -25,
};
