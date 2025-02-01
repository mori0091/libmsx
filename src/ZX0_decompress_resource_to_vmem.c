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
 * \file ZX0_decompress_resources_to_vmem.c
 */
#include "ZX0_decompress.h"

#include "resources.h"

uint32_t ZX0_decompress_resource_to_vmem(const char * path, vmemptr_t dst) {
  const ResourceIndex * res = resource_find(path);
  if (res) {
    return ZX0_decompress_bmem_to_vmem(res->offset, dst);
  }
  return 0;
}
