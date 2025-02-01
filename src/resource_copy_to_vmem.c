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
 * \file resource_copy_to_vmem.c
 */

#include "resources.h"

void resource_copy_to_vmem(const char * path, vmemptr_t dst) {
  const ResourceIndex * res = resource_find(path);
  if (res) {
    bmem_copy_to_vmem(res->offset, dst, res->size);
  }
}
