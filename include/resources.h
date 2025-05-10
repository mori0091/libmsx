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
 * \file resources.h
 * \brief Data types and functions for accessing embedded resources.
 * \ingroup RESOURCES
 */
/**
 * \defgroup RESOURCES Banked Memory as a storage of Named Resources.
 * \ingroup BANKED_MEMORY
 * `#include <resources.h>`
 *
 * @{
 */

#pragma once

#ifndef RESOURCES_H_
#define RESOURCES_H_

#include "bmem.h"

/**
 * \defgroup RESOURCES_FIND Find a resource in banked memory.
 * \ingroup RESOURCES
 * APIs for looking up a resource in banked memory by name.
 *
 * Banked memory is treated as a ROM with one large address space, and as
 * storage of named embedded resources.
 *
 * @{
 */

/**
 * Index record of an embedded resource.
 */
typedef struct ResourceIndex {
  bmemptr_t offset;             ///< Address of the resource in banked memory.
  uint32_t  size;               ///< Size of the resource.
  const char * path;            ///< Path / file name of the resource.
} ResourceIndex;

/**
 * Find an embedded resource by name.
 *
 * \param path path/file name of the resource.
 * \return a pointer to the ResourceIndex if found, otherwise NULL.
 */
const ResourceIndex * resource_find(const char * path);

/** @} */

/**
 * \defgroup RESOURCES_INDIRECT_ACCESS Reading and copying from resource in banked memory.
 * \ingroup RESOURCES
 * APIs for indirect access to banked memory by name.
 *
 * Banked memory is treated as a ROM with one large address space, and as
 * storage of named embedded resources.
 *
 * @{
 */

/**
 * Copy a resource in banked memory to VRAM.
 *
 * Searches for an embedded resource in banked memory by name and copy it into
 * the specified address in VRAM. If the resource is not found, do nothing.
 *
 * This function is same as the following code:
 * ~~~ c
 * const ResourceIndex * res = resource_find(path);
 * if (res) {
 *   bmem_copy_to_vmem(res->offset, dst, res->size);
 * }
 * ~~~
 *
 * \param path path/file name of the resource.
 * \param dst destination address of VRAM.
 *
 * \sa bmem_copy_to_vmem()
 */
void resource_copy_to_vmem(const char * path, vmemptr_t dst);

/**
 * Load a `BSAVE` formatted binary resource in banked memory into VRAM.
 *
 * Searches for embedded resources in banked memory by name and loads them into
 * VRAM as binaries in `BSAVE` format.
 *
 * Do nothing in the following cases
 * - The resource is not found,
 * - The resource is not a `BSAVE` formatted binary.
 *
 * This function is same as the following code:
 * ~~~ c
 * const ResourceIndex * res = resource_find(path);
 * if (res) {
 *   bmem_bload_s(res->offset);
 * }
 * ~~~
 *
 * \param path path/file name of the resource.
 *
 * \sa bmem_bload_s()
 */
void resource_bload_s(const char * path);

/**
 * Load a `BSAVE` formatted binary resource in banked memory into RAM.
 *
 * Searches for embedded resources in banked memory by name and loads them into
 * RAM as binaries in `BSAVE` format.
 *
 * Do nothing in the following cases
 * - The resource is not found,
 * - The resource is not a `BSAVE` formatted binary.
 * - Buffer size is too small.
 *
 * This function is same as the following code:
 * ~~~ c
 * const ResourceIndex * res = resource_find(path);
 * if (res) {
 *   bmem_bload(res->offset, buf, buf_size);
 * }
 * ~~~
 *
 * \param path     path/file name of the resource.
 * \param buf      Pointer to the RAM buffer.
 * \param buf_size Buffer size (i.e., capacity) in bytes.
 *
 * \sa bmem_bload_s()
 */
void resource_bload(const char * path, void * buf, size_t buf_size);

/** @} */

#endif // RESOURCES_H_

/** @} */
