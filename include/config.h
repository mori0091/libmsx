// -*- coding: utf-8-unix -*-
/**
 * \file config.h
 *
 * Copyright (c) 2022 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */

#pragma once

#ifndef CONFIG_H_
#define CONFIG_H_

#if !defined(__SDCC)
// omits SDCC specific keywords
#  define __at(x)
#  define __sfr char
#  define __critical
#  define __naked
#  define __banked
#endif

#endif // CONFIG_H_
