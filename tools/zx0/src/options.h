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
 * \file options.h
 */

#ifndef OPTIONS_H_
#define OPTIONS_H_

#include <stdbool.h>
#include <stddef.h>

extern bool opt_force;
extern const char * opt_suffix;
extern size_t opt_skip;
extern size_t opt_align;
extern bool opt_bsave;

void help(void);

int parse_args(int argc, char ** argv);

#endif // OPTIONS_H_
