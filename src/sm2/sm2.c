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
 * \file sm2.c
 */

#include <sm2.h>

#include "./sm2_internal.h"

#include <bmem.h>
#include <workarea.h>

// ----------------------------------------------------------------------
/** Number of sprite patterns allocated. */
uint8_t sm2__num_patterns_allocated;
/** Number of sprite planes reserved for "array" partition. */
uint8_t sm2__num_planes_reserved;
/** Number of sprite planes in use. */
uint8_t sm2__num_planes_in_use;
/** Temporal buffer / workarea. */
struct sm2__SpriteTable sm2__sprite_table;

// ----------------------------------------------------------------------
#define SM2_LIVE_CELS_MAX      (32)

typedef struct sm2__LiveCel {
  /** Key (Pointer to a cel)*/
  const sm2_Cel * ptr;
  /** Value (Base sprite pattern number assigned to the patterns of the cel)*/
  uint8_t base_pattern_number;
} sm2__LiveCel;

/** Table of cel and its base pattern nubmers. */
static sm2__LiveCel sm2__live_cels[SM2_LIVE_CELS_MAX];
static size_t sm2__num_live_cels;

// ----------------------------------------------------------------------
inline uint32_t sm2__spt_offsetof(uint8_t depth) { (void)depth; return 1UL; }
inline size_t sm2__spt_sizeof(uint8_t depth) { return (size_t)(depth * 32); }

static uint32_t sm2__sct_offsetof(uint8_t depth) { return sm2__spt_offsetof(depth) + sm2__spt_sizeof(depth); }
inline size_t sm2__sct_sizeof(uint8_t depth) { return (size_t)(depth * 16); }

static uint32_t sm2__sat_offsetof(uint8_t depth) { return sm2__sct_offsetof(depth) + sm2__sct_sizeof(depth); }
inline size_t sm2__sat_sizeof(uint8_t depth) { return  (size_t)(depth * 4); }

inline uint32_t sm2__cel_spt(const sm2_Cel * cel) { return sm2__spt_offsetof(cel->depth) + cel->addr; }
static uint32_t sm2__cel_sct(const sm2_Cel * cel) { return sm2__sct_offsetof(cel->depth) + cel->addr; }
static uint32_t sm2__cel_sat(const sm2_Cel * cel) { return sm2__sat_offsetof(cel->depth) + cel->addr; }

inline size_t sm2__cel_spt_size(const sm2_Cel * cel) { return sm2__spt_sizeof(cel->depth); }
inline size_t sm2__cel_sct_size(const sm2_Cel * cel) { return sm2__sct_sizeof(cel->depth); }
inline size_t sm2__cel_sat_size(const sm2_Cel * cel) { return sm2__sat_sizeof(cel->depth); }

// ----------------------------------------------------------------------
// static void sm2_bmem__load_spt(const sm2_Cel * cel, vmemptr_t spt) {
//   bmem_copy_to_vmem(sm2__cel_spt(cel), spt, sm2__cel_spt_size(cel));
// }
static void sm2_bmem__load_spt(const sm2_Cel * cel, vmemptr_t spt) {
  // Note that `cel->depth` must be <= SM2_SPRITE_PLANE_MAX (32).
  static uint8_t spt_buf[32 * SM2_SPRITE_PLANE_MAX];
  const size_t len = sm2__cel_spt_size(cel);
  bmem_read(sm2__cel_spt(cel), spt_buf, len);
  vmem_write(spt, spt_buf, len);
}
static void sm2_bmem__load_sct(const sm2_Cel * cel, uint8_t * sct) {
  bmem_read(sm2__cel_sct(cel), sct, sm2__cel_sct_size(cel));
}
static void sm2_bmem__load_sat(const sm2_Cel * cel, uint8_t * sat) {
  bmem_read(sm2__cel_sat(cel), sat, sm2__cel_sat_size(cel));
}

// ----------------------------------------------------------------------
inline void sm2__load_spt(const sm2_Cel * cel, vmemptr_t spt) {
  sm2_bmem__load_spt(cel, spt);
}
inline void sm2__load_sct(const sm2_Cel * cel, uint8_t * sct) {
  sm2_bmem__load_sct(cel, sct);
}
inline void sm2__load_sat(const sm2_Cel * cel, uint8_t * sat) {
  sm2_bmem__load_sat(cel, sat);
}

// ----------------------------------------------------------------------
static int8_t sm2__SPT_allocate(uint8_t cnt) {
  int8_t index = sm2__num_patterns_allocated;
  if (SM2_SPRITE_PATTERN_MAX < index + cnt) return -1;
  sm2__num_patterns_allocated += cnt;
  return index;
}
inline void sm2__SPT_deallocate_all(void) {
  sm2__num_patterns_allocated = 0;
}

// ----------------------------------------------------------------------
void sm2__LiveCel_clear_all(void) {
  sm2__num_live_cels = 0;
  sm2__SPT_deallocate_all();
}

static sm2__LiveCel * sm2__LiveCel_from(const sm2_Cel * cel) {
  sm2__LiveCel * p = &sm2__live_cels[0];
  for (uint8_t n = sm2__num_live_cels; n--; p++) {
    if (p->ptr == cel) { return p; }
  }
  if (sm2__num_live_cels < SM2_LIVE_CELS_MAX) {
    const int8_t base_idx = sm2__SPT_allocate(cel->depth);
    if (0 <= base_idx) {
      uint8_t pat = base_idx * 4;
      p->base_pattern_number = pat;
      p->ptr = cel;
      sm2__num_live_cels++;
      sm2__load_spt(cel, SPRITE_PATTERNS + pat * 8);
      return p;
    }
  }
  return NULL;
}

// ----------------------------------------------------------------------
static void sm2__set_EC_bit(struct sprite_color * c) {
  uint8_t * p = c->tagged_colors;
  for (uint8_t n = sizeof(struct sprite_color); n--; ) {
    *p++ |= SPRITE_TAG_EC;
  }
}

bool sm2__put_cel(uint8_t base_plane, const sm2_Cel * cel, int x, int y) {
  const sm2__LiveCel * const livecel = sm2__LiveCel_from(cel);
  if (!livecel) return false;
  // const sm2__LiveCel * livecel;
  // while (!(livecel = sm2__LiveCel_from(cel))) {
  //   sm2__LiveCel_clear_all();
  // }
  const uint8_t pat = livecel->base_pattern_number;

  uint8_t * const sct = (uint8_t *)&sm2__sprite_table.sct[base_plane];
  struct sprite_color * c = (struct sprite_color *)sct;
  sm2__load_sct(cel, sct);

  uint8_t * const sat = (uint8_t *)&sm2__sprite_table.sat[base_plane];
  struct sprite * s = (struct sprite *)sat;
  sm2__load_sat(cel, sat);

  const uint8_t MAGNIFIED = (RG1SAV & 1);

  for (uint8_t depth = cel->depth; depth--; ) {
    int sy = y + s->y;
    int sx = x + s->x;
    if (MAGNIFIED) {
      sy += s->y;
      sx += s->x;
    }

    if (sy < -32 || 212 <= sy) { sy = -32; }
    // Adjust Y coordinate to match BG and V-scroll offset
    sy = (sy - 1 + RG23SA) & 255;
    // Avoid y = 216, as that plane and the following will not be displayed.
    if (sy == 216) { sy = 217; }
    s->y = sy;

    if (sx < -32 || 256 <= sx) { sx = -32; }
    if (sx < 0) {
      sx += 32;
      sm2__set_EC_bit(c);
    }
    s->x = sx;

    s->pat += pat;

    s++;
    c++;
  }
  return true;
}

// ----------------------------------------------------------------------
void sm2__flush(uint8_t base_plane, uint8_t n) {
  const size_t sat_len = n * 4;
  const size_t sat_offset = base_plane * 4;
  const size_t sct_len = sat_len * 4;
  const size_t sct_offset = sat_offset * 4;
  vmem_write(SPRITE_COLORS + sct_offset, ((uint8_t *)&sm2__sprite_table.sct) + sct_offset, sct_len);
  vmem_write(SPRITES + sat_offset, ((uint8_t *)&sm2__sprite_table.sat) + sat_offset, sat_len);
}
