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
 * \file resource_find.c
 */

#include "resources.h"
#include <stddef.h>
#include <string.h>

extern const ResourceIndex libmsx__bmem_resources[];
extern const size_t libmsx__bmem_resources_length;

const ResourceIndex * resource_find(const char * path) {
  const ResourceIndex * p = &libmsx__bmem_resources[0];
  size_t n = libmsx__bmem_resources_length;
  while (n--) {
    if (!strcmp(p->path, path)) {
      return p;
    }
    p++;
  }
  return NULL;
}
