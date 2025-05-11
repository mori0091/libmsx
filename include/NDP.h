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
 * \file NDP.h
 * \brief NDP - NDP sound driver
 * \ingroup NDP
 */

#ifndef NDP_H_
#define NDP_H_

#include <memfile.h>

/**
 * \defgroup NDP NDP - NDP sound driver.
 * \ingroup LIBMSX_REPLAYER
 * `#include <NDP.h>`
 * NDP - PSG Driver for MSX (libmsx version).
 *
 * This software (NDP - PSG Driver for MSX) was originally programmed and
 * provided by naruto2413 and later modified for libmsx by Daishi Mori
 * (mori0091).
 *
 * The original NDP driver for MSX and MML editor/compiler for Windows are
 * distributed at the following site by naruto2413.
 *
 * NDP - PSG Driver for MSX
 * https://ndp.squares.net/web/
 */

/**
 * \defgroup NDP_REPLAYER The replayer APIs of the NDP sound driver.
 * \ingroup NDP
 * The replayer APIs of the NDP sound driver.
 *
 * @{
 */

/**
 * `MSX` Initialize the NDP sound driver.
 *
 * This function must be called once. In particular, it must be called
 * before the first call to any other NDP APIs.
 */
void NDP_init(void);

/**
 * `MSX` Return the version code of the NDP sound driver.
 *
 * Return the 2-byte version code `(major << 8) | (minor)`.
 *
 * If `major` is `0`, it means version “0.9.minor”,
 * otherwise it means version “major.minor”.
 *
 * For example,
 * - return `0x010A` for version 1.10
 * - return `0x0003` for version 0.9.3
 *
 * \return version code
 */
uint16_t NDP_version(void);

/**
 * `MSX` Main routine of the NDP sound driver.
 *
 * To play back background music, this function must be called at each VSYNC
 * timing.
 *
 * The easiest way is to set this function as the VSYNC interrupt handler by
 * calling set_vsync_handler().
 */
void NDP_play(void);

#if defined(LIBMSX_MEGAROM)
/*
 * An alternative version of `NDP_play()` that allows direct playback of song
 * data in MegaROM.
 */
void NDP_play_bmem(void);
#define NDP_play NDP_play_bmem
#endif

/**
 * `MSX` Start / Resume music.
 *
 * \sa NDP_fadein()
 */
void NDP_start(void);

#if defined(LIBMSX_MEGAROM)
/*
 * An alternative version of `NDP_start()` that allows direct playback of song
 * data in MegaROM.
 */
void NDP_start_bmem(void);
#define NDP_start NDP_start_bmem
#endif

/**
 * `MSX` Pause music.
 */
void NDP_pause(void);
/**
 * `MSX` Return whether paused or not.
 *
 * \return `true` if paused.
 */
bool NDP_is_paused(void);

/**
 * `MSX` Stop music.
 *
 * \sa NDP_fadeout()
 */
void NDP_stop(void);

/**
 * `MSX` Set maximum main-volume.
 *
 * \param vol  Volume (0..15)
 */
void NDP_set_volume(uint8_t vol);

/**
 * `MSX` Fade-out and stop music.
 *
 * \param wait Number of VSYNC frames per step of fade-out (wait count).
 */
void NDP_fadeout(uint8_t wait);

/**
 * `MSX` Fade-in and start music.
 *
 * \param wait Number of VSYNC frames per step of fade-in (wait count).
 */
void NDP_fadein(uint8_t wait);

/**
 * `MSX` Return whether BGM is playing or not.
 *
 * \return `true` if BGM is playing.
 */
bool NDP_is_playing(void);

/**
 * `MSX` Return status for each tracks.
 *
 * \return Status flags.
 *         - bit #3 Tracks 3 (0: finished, 1: playing)
 *         - bit #2 Tracks 2 (0: finished, 1: playing)
 *         - bit #1 Tracks 1 (0: finished, 1: playing)
 *         - bit #0 Tracks R (0: finished, 1: playing)
 */
uint8_t NDP_get_track_status(void);

/**
 * `MSX` Return the loop counter value.
 *
 * The loop counter starts at 0 and counts up when the song loops back.
 *
 * In case of overflow (exceeding 255), the counter returns to 0.
 *
 * \return The loop counter value.
 */
uint8_t NDP_get_loop_counter(void);

/** @} */

// ----------------------------------------------------------------------
/**
 * \defgroup NDP_SONG Open NDP song data, and set it in the driver.
 * \ingroup NDP
 *
 * @{
 */

/**
 * `MSX` Container of an opened NDP song data.
 */
typedef struct NDPFile {
  MemFile mf;
} NDPFile;

/**
 * `MSX` Open NDP song data stored in ROM / RAM.
 *
 * \param ndp  Pointer to a NDPFile to be initialized.
 * \param loc  Location of the NDP song data.
 * \param size Size in bytes.
 * \return     Number of songs contained in the NDP file.
 */
int NDP_open_mem(NDPFile * ndp, const uint8_t * loc, size_t size);

/**
 * `MSX` Open NDP song data stored in banked memory (MegaROM).
 *
 * \param ndp  Pointer to a NDPFile to be initialized.
 * \param loc  Location of the NDP song data.
 * \param size Size in bytes.
 * \return     Number of songs contained in the NDP file.
 */
int NDP_open_bmem(NDPFile * ndp, bmemptr_t loc, uint32_t size);

/**
 * `MSX` Open NDP file stored as named resources in banked memory (MegaROM).
 *
 * \param ndp  Pointer to a NDPFile to be initialized.
 * \param path Path/File name (*.NDP) of the resource.
 * \return     Number of songs contained in the NDP file.
 */
int NDP_open_resource(NDPFile * ndp, const char * path);

/**
 * `MSX` Setup the NDP song data to NDP sound driver.
 *
 * If the song data body is in a contiguous area of the current CPU address
 * space or within the 16KiB segment boundaries of MegaROM, set the NDP sound
 * driver to play directly from that area. Otherwise, fails.
 *
 * \param ndp  Pointer to the NDPFile opened by `NDP_open_*()`.
 *
 * \return `true` on success, `false` otherwise.
 */
bool NDP_set_bgm(NDPFile * ndp);

/**
 * `MSX` Load and setup the NDP song data to NDP sound driver.
 *
 * If the specified RAM buffer size is large enough, load the song into the
 * buffer and set the NDP sound driver to play from the buffer. Otherwise,
 * fails.
 *
 * \param ndp      Pointer to the NDPFile opened by `NDP_open_*()`.
 * \param buf      Pointer to RAM buffer.
 * \param buf_size Size of the buffer.
 *
 * \return `true` on success, `false` otherwise.
 */
bool NDP_load_bgm(NDPFile * ndp, uint8_t * buf, size_t buf_size);

/**
 * `MSX` Returns whether or not there is metadata in the NDP song data.
 *
 * \param ndp      Pointer to NDPFile opened by `NDP_open_*()`.
 *
 * \return `true` if metadata is present, `false` otherwise.
 */
bool NDP_has_metadata(NDPFile * ndp);

/**
 * `MSX` Read metadata stored in NDP song data.
 *
 * If there is metadata in the NDP song data, it is read and written to the
 * specified buffer. If successful, the buffer contents will be a concatenation
 * of five C strings (zero-terminated Shift-JIS strings).
 *
 * Each of the five string values corresponds to the following information in
 * order
 * - Title of the song
 * - Composer
 * - Arranger
 * - Programmer (NDP music data creator)
 * - Memo (remarks)
 *
 * If the buffer size is too short, some items in the buffer will be too short
 * or will be invalid strings.
 *
 * \param ndp      Pointer to NDPFile opened by `NDP_open_*()`.
 * \param buf      Pointer to the buffer into which the metadata will be read.
 * \param buf_size Size of the buffer.
 *
 * \return Size read into the buffer.
 */
size_t NDP_read_metadata(NDPFile * ndp, uint8_t * buf, size_t buf_size);

/** @} */

#endif // NDP_H_
