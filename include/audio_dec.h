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
 * \file audio_dec.h
 * \brief The audio decoder interface for the libmsx audio replayer.
 * \ingroup AUDIO
 */

#ifndef AUDIO_DEC_H_
#define AUDIO_DEC_H_

#include <stdbool.h>

/**
 * \defgroup AUDIO_DECODER The audio decoder interface for the libmsx audio replayer.
 * \ingroup AUDIO
 * `#include <audio_dec.h>`
 *
 * The audio decoder interface for the libmsx audio replayer.
 *
 * The libmsx audio replayer can handle two dedicated audio decoders:
 * - one is a background music decoder,
 * - and the other is a sound effect decoder.
 *
 * Each of the above audio decoders must implement the audio decoder interface
 * of type `AudioDecoder`.
 *
 * A music data loader or audio decoder implementation for the libmsx audio
 * replayer must provide either or both of the follwing decoder specific APIs:
 * - An API used to set the background music (BGM) to the decoder. This API also
 *   attaches the BGM decoder to the replayer by calling
 *   `audio_cb_bgm_changed()`.
 * - An API used to set the sound effects (SFX) to the decoder. This API also
 *   attaches the SFX decoder to the replayer by calling
 *   `audio_cb_sfx_changed()`.
 *
 * And it can also proivide some APIs such as:
 * - Looking up / load a music data from the storage,
 * - Get meta-data of a music (ex. default frequency, title, etc.),
 * - Turn auto-repeat feature on/off,
 * - and so on.
 *
 * \sa AUDIO_CALLBACK
 * \sa AUDIO_BUFFER
 * @{
 */

/**
 * The audio decoder interface.
 *
 * This object is used by libmsx audio replayer to decode music data.
 *
 * The libmsx audio replayer can handle two dedicated audio decoders:
 * - one is a background music decoder,
 * - and the other is a sound effect decoder.
 *
 * **Decode music data then Store in the buffer**
 *
 * The decoder decodes the music data each time it is called and stores it in
 * libmsx audio replayer's internal FIFO buffer by calling `audio_buf_put()`.
 *
 * To do this, each of the above audio decoders must implement the audio decoder
 * interface of type `AudioDecoder`.
 *
 * **Attach the decoder to the libmsx audio replayer**
 *
 * The decoder must be attached to the replayer when a music data is changed or
 * set to the decoder by the decoder specific API.
 *
 * A decoder for background music (BGM) shall be attached to the replayer by
 * `audio_cb_bgm_changed()`. So the `audio_cb_bgm_changed()` should be called
 * when the BGM is changed or set to the decoder by the decoder specific API.
 *
 * A decoder for sound effects (SFX) shall be attached to the replayer by
 * `audio_cb_sfx_changed()`. So the `audio_cb_sfx_changed()` should be called
 * when the SFX is changed or set to the decoder by the decoder specific API.
 *
 * \sa AUDIO_BUFFER
 * \sa AUDIO_CALLBACK
 */
typedef struct AudioDecoder {
  /**
   * Function pointer to the decoder I/F `decode_update()`.
   *
   * This function is called by `audio_play()` several times (depending on the
   * frequency of the replayer and VSYNC) if the decoder has been attached to
   * the replayer.
   *
   * This function shall decodes the next sampled value of the music data.
   *
   * Then, by calling the function `audio_buf_put()`, the decoded value can be
   * stored in the internal FIFO buffer of the libmsx audio replayer. (Or, it is
   * done from the function `decode_final()`.)
   *
   * If a music data has been set to the decoder and it is not reached to the
   * end of music, this shall return `true`.
   *
   * If no music data is set to the decoder or the music is already finished,
   * this shall return `false`.
   *
   * \return `true` if the music has not ended.
   *
   * \post
   * The FIFO buffer of the libmsx audio replayer may or may not be updated.
   *
   * \sa AUDIO_BUFFER
   * \sa audio_buf_put()
   */
  bool (*decode_update)(void);

  /**
   * Function pointer to the decoder I/F `decode_final()`.
   *
   * This function is called by `audio_play()` after several iterations of
   * `decode_update()` if the decoder has been attached to the replayer.
   *
   * If `decode_update()` simply decodes but does not store in the libmsx audio
   * replayer's internal FIFO buffer, the value decoded by `decode_update()`
   * should be stored by this function in the FIFO buffer.
   *
   * \post
   * The FIFO buffer of the libmsx audio replayer shall be updated / finalized.
   *
   * \sa AUDIO_BUFFER
   * \sa audio_buf_put()
   */
  void (*decode_final)(void);

  /**
   * Function pointer to the decoder I/F `set_repeat()`.
   *
   * Turn on/off the auto-repeat of the music.
   *
   * This function is called by `audio_set_repeat()` or when the decoder is
   * attached to replayer. If the decoder does not support the auto-repeat
   * function, the function pointer `set_repeat` may be set to `NULL`.
   *
   * \param repeat `true`: turn on, `false`: turn off
   */
  void (*set_repeat)(bool repeat);

} AudioDecoder;

/** @} */

#endif // AUDIO_DEC_H_
