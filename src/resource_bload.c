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
 * \file resource_bload.c
 */

#include <resources.h>

void resource_bload(const char * path, void * buf, size_t buf_size) {
  const ResourceIndex * res = resource_find(path);
  if (res) {
    bmem_bload(res->offset, buf, buf_size);
  }
}
