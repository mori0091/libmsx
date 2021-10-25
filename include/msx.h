// -*- coding: utf-8-unix -*-

#ifndef MSX_H
#define MSX_H

#pragma once

/**
 * Waits for next interrupt.
 *
 * Calling to `await_interrupt()` waits for next interrupt and blocks until the
 * interrupt would to be processed.
 */
#define await_interrupt()                       \
  __asm__("ei");                                \
  __asm__("halt")

#include "io.h"
#include "bios.h"
#include "workarea.h"

#include "vdp.h"
#include "vmem.h"

#include "sprite.h"
#include "sprite_color.h"
#include "metasprite.h"

#include "input.h"

#endif
