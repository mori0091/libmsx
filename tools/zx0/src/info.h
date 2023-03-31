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
 * \file info.h
 */

#ifndef INFO_H_
#define INFO_H_


#ifdef NDEBUG
#define DEBUG_INFO(...)
#else
#define DEBUG_INFO(...)  INFO(__VA_ARGS__)
#endif

void INFO(const char * fmt, ...);
void ERROR(const char * fmt, ...);

#endif // INFO_H_
