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
 * \file msx.h
 * \brief Using `#include <msx.h>` includes almost all C header files in
 * `libmsx`, for ease to use.
 */

#pragma once

#ifndef MSX_H
#define MSX_H

#include "config.h"

#include "interrupt.h"

#include "io.h"
#include "bios.h"
#include "workarea.h"

#include "vdp.h"
#include "vmem.h"

#include "sprite.h"
#include "sprite_color.h"
#include "metasprite.h"

#include "input.h"

#include "psg.h"
#include "sound.h"
#include "sound_eg.h"

#endif
