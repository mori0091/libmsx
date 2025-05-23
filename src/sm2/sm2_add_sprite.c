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
 * \file sm2_add_sprite.c
 */

#include <sm2.h>

void sm2_add_sprite(const sm2_Sprite * s, int x, int y) {
  const sm2_SpriteSheet * sheet = s->sheet;
  if (!sheet) return;
  const sm2_Cel * const cels = sheet->cels.ptr;
  const sm2_Frame * const frame = &sheet->frames.ptr[s->curr_frame];
  const uint8_t * cel_ids = frame->cel_ids.ptr;
  for (uint8_t n = frame->cel_ids.len; n--; cel_ids++) {
    sm2_add_cel(&cels[*cel_ids], x, y);
  }
}
