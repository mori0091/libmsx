// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2022 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file resource_bload_s.c
 */

#include "resources.h"
#include <stddef.h>

void resource_bload_s(const char * path) {
  const ResourceIndex * res = resource_find(path);
  if (res) {
    bmem_bload_s(res->offset);
  }
}
