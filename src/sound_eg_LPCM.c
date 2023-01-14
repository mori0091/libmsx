// -*- coding: utf-8-unix -*-
/**
 * \file sound_eg_LPCM.c
 *
 * Copyright (c) 2021-2023 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

#include "sound_eg_spi.h"

#define LENGTH    (self->attr.lpcm->len)
#define DATA      (self->attr.lpcm->data)

static void update(struct sound_eg * self) {
  if (LENGTH <= self->time) {
    return;
  }
  self->output = DATA[self->time++];
}

static void sound_eg_LPCM_key_on(struct sound_eg * self) {
  self->time = 0;
  update(self);
}

static void sound_eg_LPCM_key_off(struct sound_eg * self) {
  self->time = LENGTH;
  update(self);
}

static void sound_eg_LPCM_advance(struct sound_eg * self) {
  update(self);
}

const struct sound_eg_vtbl sound_eg_LPCM = {
  .key_on  = sound_eg_LPCM_key_on,
  .key_off = sound_eg_LPCM_key_off,
  .advance = sound_eg_LPCM_advance,
};
