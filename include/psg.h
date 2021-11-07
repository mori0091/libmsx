// -*- coding: utf-8-unix -*-
/**
 * \file psg.h
 *
 * Copyright (c) 2021 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project
 * https://github.com/mori0091/libmsx
 */

#ifndef PSG_H
#define PSG_H

#pragma once

#include <stdint.h>

void psg_init(void);

uint8_t psg_get(uint8_t reg);

void psg_set(uint8_t reg, uint8_t val);

#endif
