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
 * \file sm2_internal.h
 */

#ifndef SM2_INTERNAL_H_
#define SM2_INTERNAL_H_

#include "sm2.h"

#include <bios.h>

#include <stdbool.h>

// ----------------------------------------------------------------------
#define SM2_VSYNC_FREQ       (msx_get_vsync_frequency())
#define SM2_COUNTS_PER_MSEC  (SM2_VSYNC_FREQ/10)
#define SM2_COUNTS_PER_TICK  (1000/10)

// ----------------------------------------------------------------------
#define SPRITE_PATTERNS (sm2_cfg.vmem.spt) // Base address of sprite pattern generator table
#define SPRITE_COLORS   (SPRITES - 512)    // Base address of sprite color table
#define SPRITES         (sm2_cfg.vmem.sat) // Base address of sprite attribute table

// ----------------------------------------------------------------------
#define SM2_SPRITE_PATTERN_MAX (64)
#define SM2_SPRITE_PLANE_MAX   (32)

// ----------------------------------------------------------------------
struct sm2__SpriteTable {
  /** Buffer for sprite color table. */
  struct sprite_color sct[SM2_SPRITE_PLANE_MAX];
  /** Buffer for sprite attribute table. */
  struct sprite sat[SM2_SPRITE_PLANE_MAX];
};

// ----------------------------------------------------------------------
/** Number of sprite patterns allocated. */
extern uint8_t sm2__num_patterns_allocated;
/** Number of sprite planes reserved for "array" partition. */
extern uint8_t sm2__num_planes_reserved;
/** Number of sprite planes in use. */
extern uint8_t sm2__num_planes_in_use;
/** Temporal buffer / workarea. */
extern struct sm2__SpriteTable sm2__sprite_table;

// ----------------------------------------------------------------------
void sm2__LiveCel_clear_all(void);

void sm2__put_cel(uint8_t base_plane, const sm2_Cel * cel, int x, int y);

bool sm2__add_cel(uint8_t base_plane, const sm2_Cel * cel, int x, int y);

void sm2__flush(uint8_t base_plane, uint8_t n);

#endif // SM2_INTERNAL_H_
