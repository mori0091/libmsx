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
 * \file sm2_cfg.h
 * \brief SM2 (Sprite Mode 2) Runtime configuration.
 * \ingroup SM2
 */

#ifndef SM2_CFG_H_
#define SM2_CFG_H_

#include <vmem.h>

/**
 * SM2 Runtime configuration.
 */
typedef struct sm2_Config {
  struct {
    /** Sprite pattern generator table base address. */
    vmemptr_t spt;
    /** Sprite attribute table base address. */
    vmemptr_t sat;
  } vmem;
} sm2_Config;

extern sm2_Config sm2_cfg;

/**
 * `MSX2` Set VRAM address of the **working** sprite pattern generator table.
 *
 * \param spt VRAM address of the sprite pattern generator table.
 *
 * \note
 * This function does not set the VDP's VRAM memory map, but only sets the base
 * address of the sprite pattern generator table (SPT) in the VRAM to which the
 * SM2 module writes sprite patterns. To set the SPT base address of the VDP's
 * VRAM memory map, use vdp_set_sprite_pattern_table() as needed.
 *
 * \note
 * Since sm2_init() initializes the SM2 module according to the current VRAM
 * memory map settings, it is usually not necessary to call this function.
 * Therefore, use sm2_init() instead of this function unless there is a special
 * reason.
 */
inline void sm2_cfg_set_sprite_pattern_table(vmemptr_t spt) {
  sm2_cfg.vmem.spt = spt;
}

/**
 * `MSX2` Set VRAM address of the **working** sprite attribute table.
 *
 * \param sat VRAM address of the sprite attribute table.
 *
 * \note
 * This function does not set the VDP's VRAM memory map, but only sets the base
 * address of the sprite pattern generator table (SAT) in the VRAM to which the
 * SM2 module writes sprite patterns. To set the SAT base address of the VDP's
 * VRAM memory map, use vdp_set_sprite_attribute_table() as needed.
 *
 * \note
 * Since sm2_init() initializes the SM2 module according to the current VRAM
 * memory map settings, it is usually not necessary to call this function.
 * Therefore, use sm2_init() instead of this function unless there is a special
 * reason.
 */
inline void sm2_cfg_set_sprite_attribute_table(vmemptr_t sat) {
  sm2_cfg.vmem.sat = sat;
}

#endif // SM2_CFG_H_
