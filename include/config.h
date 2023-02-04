// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2023 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file config.h
 * \brief Hack for C Compiler portability.
 */

#pragma once

#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdint.h>

#if !defined(__SDCC)
// omits SDCC specific keywords
#  define __at(x)
#  define __sfr char
#  define __critical
#  define __naked
#  define __banked
#  define __sdcccall(x)
#  define MSX_IO_PORT(x) extern uint8_t
#  define MSX_BIOS(x)    extern
#else
#  define MSX_IO_PORT(x) static volatile __sfr __at (x)
#  define MSX_BIOS(x)    static __at (x)
#endif

#endif // CONFIG_H_
