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
 * \file sm2.h
 * \brief SM2 (Sprite Mode 2) Animated Color Sprites.
 * \ingroup SM2
 */
/**
 * \defgroup SM2 SM2 (Sprite Mode 2) Animated Color Sprites.
 * SM2 module provides APIs for displaying animated multicolor sprites.
 *
 * SM2 is designed to use with :
 * - [aseprite](https://www.aseprite.org/) ; Animated Sprite Editor & Pixel Art Tool
 * - [ase2msx](https://github.com/mori0091/ase2msx) ; Generates sprite data and code for libmsx/MSX2 from aseprite file.
 *
 * \sa [TUTORIAL : SM2 (Sprite Mode 2) - Animated Color Sprites](https://github.com/mori0091/libmsx/tree/main/docs/tutorial_SM2.md)
 */

#ifndef SM2_H_
#define SM2_H_

#include "sm2_cfg.h"

#include <sprite.h>
#include <sprite_color.h>

/**
 * \defgroup SM2_DATATYPES SM2 Data Types
 * \ingroup SM2
 * Sprite, Sprite-sheet, Frame, Cel, and FrameTag.
 * @{
 */

/**
 * Macro for defining anonymous slice type of array.
 * \param T element type
 */
#define sm2_Slice(T) \
  struct {           \
    T * ptr;         \
    uint8_t len;     \
  }

#define SM2_SLICE(a) {.ptr = (a), .len = sizeof(a)/sizeof((a)[0])}

/**
 * Cel - The image of the sprite animation layer.
 */
typedef struct sm2_Cel {
  /**
   * Number of sprite planes required to display the sprite cel.
   */
  uint8_t depth;
  /**
   * Base address of image data (pattern, color table, and layout
   * information).
   */
  uint32_t addr;
} sm2_Cel;

/**
 * Frame - The frame of the sprite animation.
 */
typedef struct sm2_Frame {
  /** Duration of animation frame in milliseconds. */
  size_t duration;
  /** List of cel numbers for each layer. */
  sm2_Slice(const uint8_t) cel_ids;
} sm2_Frame;

/**
 * SpriteSheet - Whole animation frames and cels of a sprite sheet.
 */
typedef struct sm2_SpriteSheet {
  /** List of animation frames. */
  sm2_Slice(const sm2_Frame) frames;
  /** List of whole cels. */
  sm2_Slice(const sm2_Cel) cels;
} sm2_SpriteSheet;

/**
 * Enumeration of animation direction.
 */
enum SM2_Direction {
  SM2_FORWARD,
  SM2_REVERSE,
  SM2_PINGPONG,
  SM2_PINGPONG_REVERSE,
};

/**
 * FrameTag - Definition of a range of animation frames.
 */
typedef struct sm2_FrameTag {
  uint8_t from;              ///< 1st frame number.
  uint8_t to;                ///< last frame number.
  uint8_t direction;         ///< Animation direction.
  size_t repeats;            ///< Number of repeats; 0 means inf.
} sm2_FrameTag;

/**
 * Sprite - Animation state of a sprite.
 */
typedef struct sm2_Sprite {
  const sm2_SpriteSheet * sheet; ///< Sprite sheet.
  const sm2_FrameTag * tag;      ///< Current range of animation frames.
  uint8_t curr_frame;            ///< Index of current frame.
  uint8_t curr_direction;        ///< Current animation direction; forward/reverse.
  size_t remaining_duration;     ///< Remaining duration of the current frame.
  size_t repeats;                ///< Repeat count; Number of loops finished.
} sm2_Sprite;

/** @} */

/**
 * \defgroup SM2_INIT SM2 initialization for SM2 module.
 * \ingroup SM2
 * Initialization for SM2 module.
 * @{
 */

/**
 * `MSX2` Initialize SM2 module.
 *
 * Initialize and configure SM2 module based on the current VRAM memory map.
 */
void sm2_init(void);

/** @} */

/**
 * \defgroup SM2_ANIMATION SM2 updates animation state.
 * \ingroup SM2
 * Updates animation state of Animated Color Sprites.
 * @{
 */

/**
 * `MSX2` Construction and (re)initialization of the animated color sprite.
 *
 * \param s     Sprite object to be (re-)initialized.
 * \param sheet Sprite-sheet object.
 * \param tag   Frame-tag object that defines the range of animation frames.
 *
 * \note
 * This function can be used not only for initialization but also for switching
 * the animation pattern (sprite sheet and/or tag) of a sprite object.
 */
void sm2_init_sprite(sm2_Sprite * s, const sm2_SpriteSheet * sheet, const sm2_FrameTag * tag);

/**
 * `MSX2` Update the animation state of sprite object.
 *
 * \param s     Sprite object.
 *
 * \note
 * In order to update the animation state of the sprite object,
 * sm2_update_sprite() and sm2_update_sprites() should be called periodically
 * (as much as possible at each VSYNC timing).
 *
 * \note
 * Both sm2_update_sprite() and sm2_update_sprites() can be called from the
 * VSYNC interrupt handler.
 */
void sm2_update_sprite(sm2_Sprite * s);

/**
 * `MSX2` Update the animation state of a series of sprite objects.
 *
 * \param ss    An array of sprite objects.
 * \param num   Number of elements of the array.
 *
 * \note
 * In order to update the animation state of the sprite object,
 * sm2_update_sprite() and sm2_update_sprites() should be called periodically
 * (as much as possible at each VSYNC timing).
 *
 * \note
 * Both sm2_update_sprite() and sm2_update_sprites() can be called from the
 * VSYNC interrupt handler.
 */
inline void sm2_update_sprites(sm2_Sprite ss[], size_t num) {
  while (num--) {
    sm2_update_sprite(ss);
    ss++;
  }
}

/** @} */

/**
 * \defgroup SM2_DISPLAY SM2 display Animated Color Sprites.
 * \ingroup SM2
 * Display Animated Color Sprites.
 * @{
 */

/**
 * `MSX2` Display the sprite cels registered in the internal buffer.
 *
 * Display (write to VRAM) all sprite cels registered in the internal buffer by
 * sm2_add_sprite() or sm2_add_cel(). After that the buffer is cleared.
 *
 * \sa sm2_add_sprite()
 * \sa sm2_add_cel()
 */
void sm2_flush(void);

/**
 * `MSX2` Add the cels of the current frame of the sprite object to the internal
 * buffer.
 *
 * \param s          Sprite object.
 * \param x          X-coordinate.
 * \param y          Y-coordinate.
 *
 * \sa sm2_flush()
 *
 * \note
 * To display or move sprites added to the internal buffer by sm2_add_sprite()
 * or sm2_add_cel() on screen, sm2_flush() must be called.
 */
void sm2_add_sprite(const sm2_Sprite * s, int x, int y);

/**
 * `MSX2` Add a sprite cel to the internal buffer.
 *
 * \param cel        Sprite cel.
 * \param x          X-coordinate.
 * \param y          Y-coordinate.
 *
 * \sa sm2_flush()
 *
 * \note
 * To display or move sprites added to the internal buffer by sm2_add_sprite()
 * or sm2_add_cel() on screen, sm2_flush() must be called.
 */
void sm2_add_cel(const sm2_Cel * cel, int x, int y);

/**
 * `MSX2` Reserves some sprite planes for direct access.
 *
 * V9938 / V9958 VDP has 32 sprite planes for displaying sprites.
 *
 * To display a multicolor sprite (sm2_Sprite or sm2_Cel), multiple sprite
 * planes are required. Normally, the SM2 module assigns a sprite plane to each
 * sprite cel in the call order of sm2_add_sprite() or sm2_add_cel().
 *
 * However, the application developer may wish to specify the sprite plane
 * number directly.
 *
 * In such a case, the sprite plane can be reserved by calling this function. To
 * place a sprite in the reserved sprite plane directly, use sm2_put_sprite() or
 * sm2_put_cel() after sm2_reserve().
 *
 * The number of reserved sprite planes is initially set to `0`.
 *
 * \param n The number of reserved sprite planes. (0 <= n <= 32)
 *
 * \sa sm2_put_sprite()
 * \sa sm2_put_cel()
 * \sa sm2_get_reserved()
 */
void sm2_reserve(uint8_t n);

/**
 * `MSX2` Returns the number of reserved sprite planes.
 *
 * \return The number of reserved sprite planes.
 *
 * \sa sm2_reserve()
 */
uint8_t sm2_get_reserved(void);

/**
 * `MSX2` Display the cels of the current frame of the sprite object.
 *
 * The plane numbers from `base_plane` to `base_plane + N - 1` must be reserved
 * (i.e., `base_plane + N < sm2_get_reserved()`). Otherwise, nothing is done.
 *
 * Here, `N` is the sum of the depths of all cels in the current frame of the
 * sprite.
 *
 * \param base_plane The 1st plane number.
 * \param s          Sprite object.
 * \param x          X-coordinate.
 * \param y          Y-coordinate.
 *
 * \sa sm2_reserve()
 */
void sm2_put_sprite(uint8_t base_plane, const sm2_Sprite * s, int x, int y);

/**
 * `MSX2` Display the sprite cel.
 *
 * The plane numbers from `base_plane` to `base_plane + cel->depth - 1` must be
 * reserved (i.e., `base_plane + cel->depth < sm2_get_reserved()`). Otherwise,
 * nothing is done.
 *
 * \param base_plane The 1st plane number.
 * \param cel        Sprite cel.
 * \param x          X-coordinate.
 * \param y          Y-coordinate.
 *
 * \sa sm2_reserve()
 */
void sm2_put_cel(uint8_t base_plane, const sm2_Cel * cel, int x, int y);

/** @} */

#endif // SM2_H_
