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
 * \file array.c
 */

#include "array.h"

#include <stdio.h>
#include <stdlib.h>

void * array(size_t length, size_t element_size) {
  void * ret = calloc(length, element_size);
  if (!ret) {
    perror("not enough memory");
    exit(1);
  }
  return ret;
}
