// -*- coding: utf-8-unix -*-
/**
 * \file workarea.h
 *
 * Copyright (c) 2021 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */

#pragma once

#ifndef WORKAREA_H
#define WORKAREA_H

#include <stdint.h>

#include "config.h"

/** key click beep swith (0:off, non-zero:on) */
static volatile __at (0xf3db) uint8_t CLIKSW;

static volatile __at (0xf3df) uint8_t RG0SAV;
static volatile __at (0xf3e0) uint8_t RG1SAV;
static volatile __at (0xf3e1) uint8_t RG2SAV;
static volatile __at (0xf3e2) uint8_t RG3SAV;
static volatile __at (0xf3e3) uint8_t RG4SAV;
static volatile __at (0xf3e4) uint8_t RG5SAV;
static volatile __at (0xf3e5) uint8_t RG6SAV;
static volatile __at (0xf3e6) uint8_t RG7SAV;

static volatile __at (0xffe7) uint8_t RG8SAV;
static volatile __at (0xffe8) uint8_t RG9SAV;
static volatile __at (0xffe9) uint8_t RG10SA;
static volatile __at (0xffea) uint8_t RG11SA;
static volatile __at (0xffeb) uint8_t RG12SA;
static volatile __at (0xffec) uint8_t RG13SA;
static volatile __at (0xffed) uint8_t RG14SA;
static volatile __at (0xffee) uint8_t RG15SA;
static volatile __at (0xffef) uint8_t RG16SA;
static volatile __at (0xfff0) uint8_t RG17SA;
static volatile __at (0xfff1) uint8_t RG18SA;
static volatile __at (0xfff2) uint8_t RG19SA;
static volatile __at (0xfff3) uint8_t RG20SA;
static volatile __at (0xfff4) uint8_t RG21SA;
static volatile __at (0xfff5) uint8_t RG22SA;
static volatile __at (0xfff6) uint8_t RG23SA;

static volatile __at (0xfffa) uint8_t RG25SA;
static volatile __at (0xfffb) uint8_t RG26SA;
static volatile __at (0xfffc) uint8_t RG27SA;

#endif
