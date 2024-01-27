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
 * \file SCC_WAVEFORM_TRIANGLE.c
 */

#include "scc_wav.h"

const int8_t SCC_WAVEFORM_TRIANGLE[32] = {
     0,  +16,  +32,  +48,  +64,  +80,  +96, +112,
  +127, +112,  +96,  +80,  +64,  +48,  +32,  +16,
     0,  -16,  -32,  -48,  -64,  -80,  -96, -112,
  -128, -112,  -96,  -80,  -64,  -48,  -32,  -16,
};
