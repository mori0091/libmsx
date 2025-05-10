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
 * \file NDP_open_resource.c
 */

#include <NDP.h>
#include <resources.h>

extern int NDP__verify(MemFile * mf);

int NDP_open_resource(NDPFile * ndp, const char * path) {
  const ResourceIndex * r = resource_find(path);
  if (!r || bmem_get(r->offset) != 0xfe) return 0;
  const size_t size = bmem_get_u16(r->offset + 3) - bmem_get_u16(r->offset + 1) + 1;
  if (size < r->size - 7) return 0;
  return NDP_open_bmem(ndp, r->offset+7, size);
}
