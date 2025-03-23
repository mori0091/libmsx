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
 * \file memfile.h
 * \brief Stream interface for ROM / RAM / MegaROM.
 * \ingroup STREAM
 */
/**
 * \defgroup STREAM Stream interface for ROM / RAM / MegaROM.
 * \ingroup LIBMSX_STORAGE
 * `#include <memfile.h>`
 *
 * Provides a stream interface similar to the standard C library I/O streams for
 * accessing ROM/RAM/MegaROM.
 *
 * @{
 */

#ifndef MEMFILE_H_
#define MEMFILE_H_

#include <bmem.h>
#include <stdbool.h>
#include <stdint.h>

/**
 * \defgroup STREAM_OPEN Open stream.
 * \brief Open whole or part of memory image as stream.
 * \ingroup STREAM
 */
/**
 * \defgroup STREAM_ERR Check error status of the stream.
 * \brief Check error status of the stream.
 * \ingroup STREAM
 */
/**
 * \defgroup STREAM_READ Read from the current position of the stream.
 * \brief Read from the current position of the stream.
 * \ingroup STREAM
 */
/**
 * \defgroup STREAM_SEEK Get / Set the current position of the stream.
 * \brief Get / Set the current position of the stream.
 * \ingroup STREAM
 */

/**
 * A constant to specify that the offset is relative to the start of the stream.
 * \ingroup STREAM_SEEK
 * \sa mfseek()
 */
#define MEM_SEEK_SET  (0)

/**
 * A constant to specify that the offset is relative to the current position indicator.
 * \ingroup STREAM_SEEK
 * \sa mfseek()
 */
#define MEM_SEEK_CUR  (1)

/**
 * A constant to specify that the offset is relative to the end of the stream.
 * \ingroup STREAM_SEEK
 * \sa mfseek()
 */
#define MEM_SEEK_END  (2)

/**
 * Stream position indicator.
 * \ingroup STREAM_SEEK
 * \note Corresponds to `fpos_t` type of the standard C library I/O stream.
 */
typedef struct MemPos mempos_t;

/**
 * Stream pointer.
 *
 * \note Corresponds to `FILE` type of the standard C library I/O stream.
 */
typedef struct MemFile MemFile;

/**
 * Open memory image in ROM/RAM as stream.
 * \ingroup STREAM_OPEN
 *
 * \param mf   Pointer to stream pointer to be initialized.
 * \param p    Location of the memory image.
 * \param size Size in bytes.
 */
void mfopen_mem(MemFile * mf, uint8_t * p, size_t size);

/**
 * Open memory image in banked memory (MegaROM) as stream.
 * \ingroup STREAM_OPEN
 *
 * \param mf   Pointer to stream pointer to be initialized.
 * \param p    Location of the memory image.
 * \param size Size in bytes.
 *
 * \sa BANKED_MEMORY
 * \sa RESOURCES
 */
void mfopen_bmem(MemFile * mf, bmemptr_t p, uint32_t size);

/**
 * Open a new stream with the specified size range from the current position.
 * \ingroup STREAM_OPEN
 *
 * \param dst  Pointer to stream pointer to be initialized.
 * \param src  Pointer to opened stream pointer.
 * \param size Size in bytes. (size of the range)
 */
void mfslice(MemFile * dst, const MemFile * src, long size);

/**
 * Return `true` if reached to the end of stream.
 * \ingroup STREAM_ERR
 *
 * \param mf  Pointer to opened stream pointer.
 * \return    `true` if end-of-file.
 */
bool mfeof(MemFile * mf);

/**
 * Read the next byte from stream pointed by `mf`.
 * \ingroup STREAM_READ
 *
 * \param mf  Pointer to opened stream pointer.
 * \return    The byte read.
 */
uint8_t mfread_u8(MemFile * mf);

/**
 * Read some bytes from stream pointed by `mf`.
 * \ingroup STREAM_READ
 *
 * \param mf   Pointer to opened stream pointer.
 * \param ptr  Pointer to buffer into which bytes are read.
 * \param size Expected number of bytes.
 * \return     Actual number of bytes read.
 */
size_t mfread(MemFile * mf, void * ptr, size_t size);

/**
 * Read the next 16-bit of little-endian value from stream pointed by `mf`.
 * \ingroup STREAM_READ
 *
 * \param mf  Pointer to opened stream pointer.
 * \return    The 16-bit value read.
 */
inline uint16_t mfread_u16(MemFile * mf) {
  uint16_t x;
  mfread(mf, &x, sizeof(x));
  return x;
}

/**
 * Read the next 32-bit of little-endian value from stream pointed by `mf`.
 * \ingroup STREAM_READ
 *
 * \param mf  Pointer to opened stream pointer.
 * \return    The 32-bit value read.
 */
inline uint32_t mfread_u32(MemFile * mf) {
  uint32_t x;
  mfread(mf, &x, sizeof(x));
  return x;
}

/**
 * Set the stream position indicator for the stream pointed by `mf`.
 * \ingroup STREAM_SEEK
 *
 * \param mf     Pointer to opened stream pointer.
 * \param offset Offset in bytes from the position specified by `whence`.
 * \param whence Specify the base position of offset.
 *               - MEM_SEEK_SET specifies that the offset is relative to the start of the stream,
 *               - MEM_SEEK_CUR specifies that the offset is relative to the current position indicator.
 *               - MEM_SEEK_END specifies that the offset is relative to the end of the stream.
 */
void mfseek(MemFile * mf, long offset, int whence);

/**
 * Get the stream position indicator of the stream pointed by `mf`.
 * \ingroup STREAM_SEEK
 *
 * \param mf     Pointer to opened stream pointer.
 * \param pos    Pointer to the buffer where the stream position indicator will be stored.
 */
void mfgetpos(MemFile * mf, mempos_t * pos);

/**
 * Set the stream position indicator of the stream pointed by `mf`.
 * \ingroup STREAM_SEEK
 *
 * \param mf     Pointer to opened stream pointer.
 * \param pos    Pointer to the stream position indicator retrieved by mfgetpos().
 */
void mfsetpos(MemFile * mf, const mempos_t * pos);

/**
 * Set the stream position indicator of the stream pointed by `mf`.
 * \ingroup STREAM_SEEK
 *
 * \param mf     Pointer to opened stream pointer.
 * \param base   Pointer to the stream position indicator retrieved by mfgetpos().
 * \param offset Offset from the `base`.
 */
void mfsetpos2(MemFile * mf, const mempos_t * base, long offset);


#include "./internal/memfile_types.h"

/** @} */

#endif // MEMFILE_H_
