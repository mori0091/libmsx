// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2024 Daishi Mori (mori0091)
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

#include <stdio.h>
#include <stdlib.h>

#ifdef NDEBUG
#define DEBUG_INFO(...)
#else
#define DEBUG_INFO(...)  INFO(__VA_ARGS__)
#endif

#define INFO(...)        fprintf(stderr, __VA_ARGS__)
#define ERROR(...)                              \
    do {                                        \
        fprintf(stderr, __VA_ARGS__);           \
        fprintf(stderr, "\n");                  \
        exit(1);                                \
    } while (0)

#endif // INFO_H_
