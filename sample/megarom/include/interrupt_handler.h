// -*- coding: utf-8-unix -*-
/**
 * \file interrupt_handler.h
 *
 * Copyright (c) 2022 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

#pragma once

#ifndef INTERRUPT_HANDLER_H_
#define INTERRUPT_HANDLER_H_

#include <stdint.h>

void set_hscroll(uint16_t x);

void setup_interrupt(void);

#endif  // INTERRUPT_HANDLER_H_
