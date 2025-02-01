// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2025 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file la0_open_resource.c
 */

#include "la0__internal.h"
#include "memfile.h"

#include <resources.h>

int la0_open_resource(MemFile * mf, const char * path) {
  if (!path) return 0;
  const ResourceIndex * res = resource_find(path);
  if (!res) return 0;
  return la0_open_bmem(mf, res->offset, res->size);
}
