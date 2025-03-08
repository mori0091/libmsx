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
 * \file sm2_put_sprite.c
 */

#include <sm2.h>

void sm2_put_sprite(uint8_t base_plane, const sm2_Sprite * s, int x, int y) {
  const sm2_Cel * const cels = s->sheet->cels.ptr;
  const sm2_Frame * const frame = &s->sheet->frames.ptr[s->curr_frame];
  const uint8_t * cel_ids = frame->cel_ids.ptr;
  for (uint8_t n = frame->cel_ids.len; n--; cel_ids++) {
    const sm2_Cel * cel = &cels[*cel_ids];
    sm2_put_cel(base_plane, cel, x, y);
    base_plane += cel->depth;
  }
}
