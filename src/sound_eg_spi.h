// -*- coding: utf-8-unix -*-
/**
 * \file sound_eg_spi.h
 *
 * Copyright (c) 2021-2025 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */
#ifndef SOUND_EG_SPI_H
#define SOUND_EG_SPI_H

#pragma once

#include "sound_eg.h"

/**
 * Software envelope generator object.
 *
 * \note
 * This is used by the sound driver internally. Not for the user application.
 */
struct sound_eg {
  /** class identifier and instance constants. */
  struct sound_eg_attribute attr;
  // instance fields
  /** current output signal level of the envelope (0..255) */
  uint8_t output;
  /** identifier of current state */
  uint8_t state;
  /** elapsed time or remaining duration of current state */
  uint8_t time;
};

/**
 * Instance methods (set of function pointers) of software envelope generator
 * objects.
 *
 * \note
 * This is used by the sound driver internally. Not for the user application.
 */
struct sound_eg_vtbl {
  /** Notify `key on` to the envelope generator. */
  void (* key_on)(struct sound_eg * self);
  /** Notify `key off` to the envelope generator. */
  void (* key_off)(struct sound_eg * self);
  /** Notifies the envelope generator that 1/60th of a second has elapsed. */
  void (* advance)(struct sound_eg * self);
};

#endif
