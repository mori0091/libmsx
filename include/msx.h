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

/** \defgroup LIBMSX_SYS         BIOS, Interrupts, Slots, etc. */
/** \defgroup LIBMSX_HID         Human Interaction Device. */
/** \defgroup LIBMSX_CUI         Character User Interface. */
/** \defgroup LIBMSX_GRAPHICS    Video / Display / Graphics. */
/** \defgroup LIBMSX_IMAGE_PROC  Image processing. */
/** \defgroup LIBMSX_STORAGE     RAM/ROM/MegaROM Storage Interface. */
/** \defgroup LIBMSX_REPLAYER    Audio/Sound drivers. */
/** \defgroup LIBMSX_SOUNDCHIP   Sound devices. */
/** \defgroup LIBMSX_COMPRESS    Compression / Decompression. */

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
