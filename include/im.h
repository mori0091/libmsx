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
 * \file im.h
 * \brief 8x8px, 16x16px bitmap image processing.
 * \ingroup IM
 */

/**
 * \defgroup IM 8x8px, 16x16px bitmap image processing.
 * `#include <im.h>`
 */

/**
 * \defgroup IM_TRANSFORM_BM8x8 Matrix transformation operators for 8x8px 1bpp bitmap images.
 *
 * \ingroup IM
 *
 * Matrix transformation operators for 8x8px 1bpp bitmap images.
 *
 * | OPERATOR                  | ROM/RAM to RAM      | ROM/RAM to VRAM        |
 * |---------------------------|---------------------|------------------------|
 * | COPY                      | im\_copy\_BM8x8()   | im\_copy\_v\_BM8x8()   |
 * | FLIP HORIZONTALLY         | im\_hflip\_BM8x8()  | im\_hflip\_v\_BM8x8()  |
 * | FLIP VERTICALLY           | im\_vflip\_BM8x8()  | im\_vflip\_v\_BM8x8()  |
 * | TRANSPOSE                 | im\_tr\_BM8x8()     | im\_tr\_v\_BM8x8()     |
 * | ANTI-DIAGONAL TRANSPOSE   | im\_adtr\_BM8x8()   | im\_adtr\_v\_BM8x8()   |
 * | ROTATE 90 DEG. CLOCKWISE  | im\_rot90\_BM8x8()  | im\_rot90\_v\_BM8x8()  |
 * | ROTATE 180 DEG.           | im\_rot180\_BM8x8() | im\_rot180\_v\_BM8x8() |
 * | ROTATE 270 DEG. CLOCKWISE | im\_rot270\_BM8x8() | im\_rot270\_v\_BM8x8() |
 */

/**
 * \defgroup IM_BOOLEAN_BM8x8 Boolean operators for 8x8px 1bpp bitmap images.
 *
 * \ingroup IM
 *
 * Boolean operators for 8x8px 1bpp bitmap images.
 *
 * | OPERATOR                 | ROM/RAM to RAM    | ROM/RAM to VRAM      |
 * |--------------------------|-------------------|----------------------|
 * | BITWISE NOT (COMPLEMENT) | im\_cmpl\_BM8x8() | im\_cmpl\_v\_BM8x8() |
 * | BITWISE AND              | im\_and\_BM8x8()  | im\_and\_v\_BM8x8()  |
 * | BITWISE OR               | im\_or\_BM8x8()   | im\_or\_v\_BM8x8()   |
 * | BITWISE XOR              | im\_xor\_BM8x8()  | im\_xor\_v\_BM8x8()  |
 * | PIXELWISE SET-DIFFERENCE | im\_diff\_BM8x8() | im\_diff\_v\_BM8x8() |
 */

/**
 * \defgroup IM_SHIFT_BM8x8 Shift operators for 8x8px 1bpp bitmap images.
 *
 * \ingroup IM
 *
 * Shift / Rotate (Scroll) operators for 8x8px 1bpp bitmap images.
 *
 * | OPERATOR            | ROM/RAM to RAM              | ROM/RAM to VRAM           |
 * |---------------------|-----------------------------|---------------------------|
 * | SHIFT UPWARD        | im\_shift\_u\_BM8x8()       | im\_shift\_u\_v\_BM8x8()  |
 * | SHIFT DOWNWARD      | im\_shift\_d\_BM8x8()       | im\_shift\_d\_v\_BM8x8()  |
 * | SHIFT LEFT          | im\_shift\_l\_BM8x8()       | im\_shift\_l\_v\_BM8x8()  |
 * | SHIFT RIGHT         | im\_shift\_r\_BM8x8()       | im\_shift\_r\_v\_BM8x8()  |
 * | ROTATE UPWARD       | im\_rotate\_u\_BM8x8()      | im\_rotate\_u\_v\_BM8x8() |
 * | ROTATE DOWNWARD     | im\_rotate\_d\_BM8x8()      | im\_rotate\_d\_v\_BM8x8() |
 * | ROTATE LEFT         | im\_rotate\_l\_BM8x8()      | im\_rotate\_l\_v\_BM8x8() |
 * | ROTATE RIGHT        | im\_rotate\_r\_BM8x8()      | im\_rotate\_r\_v\_BM8x8() |
 * | VECTOR SHIFT LEFT   | im\_shift\_l\_vec\_BM8x8()  | n/a                       |
 * | VECTOR SHIFT RIGHT  | im\_shift\_r\_vec\_BM8x8()  | n/a                       |
 * | VECTOR ROTATE LEFT  | im\_rotate\_l\_vec\_BM8x8() | n/a                       |
 * | VECTOR ROTATE RIGHT | im\_rotate\_r\_vec\_BM8x8() | n/a                       |
 */

/**
 * \defgroup IM_TILEMAP_BM8x8 Tile-mapping for 8x8px 1bpp bitmap images.
 *
 * \ingroup IM
 *
 * Tile-mapping for 8x8px 1bpp bitmap images.
 *
 * | OPERATOR     | ROM/RAM to RAM       | ROM/RAM to VRAM         |
 * |--------------|----------------------|-------------------------|
 * | TILE MAPPING | im\_tilemap\_BM8x8() | im\_tilemap\_v\_BM8x8() |
 */


/**
 * \defgroup IM_TRANSFORM_SP16x16 Matrix transformation operators for 16x16 1bpp sprite patterns.
 *
 * \ingroup IM
 *
 * Matrix transformation operators for 16x16 1bpp sprite patterns.
 *
 * | OPERATOR                  | ROM/RAM to RAM        | ROM/RAM to VRAM          |
 * |---------------------------|-----------------------|--------------------------|
 * | COPY                      | im\_copy\_SP16x16()   | im\_copy\_v\_SP16x16()   |
 * | FLIP HORIZONTALLY         | im\_hflip\_SP16x16()  | im\_hflip\_v\_SP16x16()  |
 * | FLIP VERTICALLY           | im\_vflip\_SP16x16()  | im\_vflip\_v\_SP16x16()  |
 * | TRANSPOSE                 | im\_tr\_SP16x16()     | im\_tr\_v\_SP16x16()     |
 * | ROTATE 90 DEG. CLOCKWISE  | im\_rot90\_SP16x16()  | im\_rot90\_v\_SP16x16()  |
 * | ROTATE 180 DEG.           | im\_rot180\_SP16x16() | im\_rot180\_v\_SP16x16() |
 * | ROTATE 270 DEG. CLOCKWISE | im\_rot270\_SP16x16() | im\_rot270\_v\_SP16x16() |
 * | ANTIDIAGONAL TRANSPOSE    | im\_adtr\_SP16x16()   | im\_adtr\_v\_SP16x16()   |
 */

/**
 * \defgroup IM_BOOLEAN_SP16x16 Boolean operators for 16x16 1bpp sprite patterns.
 *
 * \ingroup IM
 *
 * Boolean operators for 16x16 1bpp sprite patterns.
 *
 * | OPERATOR                 | ROM/RAM to RAM      | ROM/RAM to VRAM        |
 * |--------------------------|---------------------|------------------------|
 * | BITWISE NOT (COMPLEMENT) | im\_cmpl\_SP16x16() | im\_cmpl\_v\_SP16x16() |
 * | BITWISE AND              | im\_and\_SP16x16()  | im\_and\_v\_SP16x16()  |
 * | BITWISE OR               | im\_or\_SP16x16()   | im\_or\_v\_SP16x16()   |
 * | BITWISE XOR              | im\_xor\_SP16x16()  | im\_xor\_v\_SP16x16()  |
 * | PIXELWISE SET-DIFFERENCE | im\_diff\_SP16x16() | im\_diff\_v\_SP16x16() |
 */

/**
 * \defgroup IM_SHIFT_SP16x16 Shift operators for 16x16 1bpp sprite patterns.
 *
 * \ingroup IM
 *
 * Shift / Rotate (Scroll) operators for 16x16 1bpp sprite patterns.
 *
 * | OPERATOR            | ROM/RAM to RAM           | ROM/RAM to VRAM             |
 * |---------------------|--------------------------|-----------------------------|
 * | SHIFT UPWARD        | im\_shift\_u\_SP16x16()  | im\_shift\_u\_v\_SP16x16()  |
 * | SHIFT DOWNWARD      | im\_shift\_d\_SP16x16()  | im\_shift\_d\_v\_SP16x16()  |
 * | SHIFT LEFT          | im\_shift\_l\_SP16x16()  | im\_shift\_l\_v\_SP16x16()  |
 * | SHIFT RIGHT         | im\_shift\_r\_SP16x16()  | im\_shift\_r\_v\_SP16x16()  |
 * | ROTATE UPWARD       | im\_rotate\_u\_SP16x16() | im\_rotate\_u\_v\_SP16x16() |
 * | ROTATE DOWNWARD     | im\_rotate\_d\_SP16x16() | im\_rotate\_d\_v\_SP16x16() |
 * | ROTATE LEFT         | im\_rotate\_l\_SP16x16() | im\_rotate\_l\_v\_SP16x16() |
 * | ROTATE RIGHT        | im\_rotate\_r\_SP16x16() | im\_rotate\_r\_v\_SP16x16() |
 */

/**
 * \defgroup IM_TRIM_SP16x16 Trimming (Cropping) for 16x16 1bpp sprite patterns.
 *
 * \ingroup IM
 *
 * Trimming (Cropping) for 16x16 1bpp sprite patterns.
 *
 * | OPERATOR            | ROM/RAM to RAM      | ROM/RAM to VRAM |
 * |---------------------|---------------------|-----------------|
 * | TRIMMING (CROPPING) | im\_trim\_SP16x16() | n/a             |
 */

#ifndef IMG_H_
#define IMG_H_

#include <stddef.h>
#include <stdint.h>

/**
 * 8x8 pixels 1bpp bitmap image (8 rows x 1 byte/row).
 *
 * \ingroup IM
 */
typedef uint8_t BM8x8[8];

/**
 * 8x8 pixels 1bpp sprite pattern (Type alias for BM8x8).
 *
 * \ingroup IM
 */
typedef BM8x8 SP8x8;

/**
 * 16x16 pixels 1bpp sprite pattern (Four 8x8px 1bpp images, top left, bottom
 * left, top right, bottom right).
 *
 * \ingroup IM
 */
typedef BM8x8 SP16x16[4];

/**
 * `MSX` Copy an 8x8 pixels 1bpp image.
 *
 * \ingroup IM_TRANSFORM_BM8x8
 *
 * \param src Pointer to source image.
 * \param dst Pointer to destination image.
 */
void im_copy_BM8x8(const BM8x8 * src, BM8x8 * dst);

/**
 * `MSX` Copy an 8x8 pixels 1bpp image.
 *
 * \ingroup IM_TRANSFORM_BM8x8
 *
 * \param src Pointer to source image.
 *
 * \note
 * The destination VRAM address must be set in advance with
 * vmem_set_write_address().
 */
void im_copy_v_BM8x8(const BM8x8 * src);

/**
 * `MSX` Flip an 8x8 pixels 1bpp image horizontally.
 *
 * \ingroup IM_TRANSFORM_BM8x8
 *
 * \param src Pointer to source image.
 * \param dst Pointer to destination image.
 */
void im_hflip_BM8x8(const BM8x8 * src, BM8x8 * dst);

/**
 * `MSX` Flip an 8x8 pixels 1bpp image horizontally.
 *
 * \ingroup IM_TRANSFORM_BM8x8
 *
 * \param src Pointer to source image.
 *
 * \note
 * The destination VRAM address must be set in advance with
 * vmem_set_write_address().
 */
void im_hflip_v_BM8x8(const BM8x8 * src);

/**
 * `MSX` Flip an 8x8 pixels 1bpp image vertically.
 *
 * \ingroup IM_TRANSFORM_BM8x8
 *
 * \param src Pointer to source image.
 * \param dst Pointer to destination image.
 */
void im_vflip_BM8x8(const BM8x8 * src, BM8x8 * dst);

/**
 * `MSX` Flip an 8x8 pixels 1bpp image vertically.
 *
 * \ingroup IM_TRANSFORM_BM8x8
 *
 * \param src Pointer to source image.
 *
 * \note
 * The destination VRAM address must be set in advance with
 * vmem_set_write_address().
 */
void im_vflip_v_BM8x8(const BM8x8 * src);

/**
 * `MSX` Transpose an 8x8 pixels 1bpp image around the main diagonal.
 *
 * \ingroup IM_TRANSFORM_BM8x8
 *
 * \param src Pointer to source image.
 * \param dst Pointer to destination image.
 */
void im_tr_BM8x8(const BM8x8 * src, BM8x8 * dst);

/**
 * `MSX` Transpose an 8x8 pixels 1bpp image around the main diagonal.
 *
 * \ingroup IM_TRANSFORM_BM8x8
 *
 * \param src Pointer to source image.
 *
 * \note
 * The destination VRAM address must be set in advance with
 * vmem_set_write_address().
 */
void im_tr_v_BM8x8(const BM8x8 * src);

/**
 * `MSX` Transpose an 8x8 pixels 1bpp image around the **antidiagonal**.
 *
 * \ingroup IM_TRANSFORM_BM8x8
 *
 * \param src Pointer to source image.
 * \param dst Pointer to destination image.
 */
void im_adtr_BM8x8(const BM8x8 * src, BM8x8 * dst);

/**
 * `MSX` Transpose an 8x8 pixels 1bpp image around the **antidiagonal**.
 *
 * \ingroup IM_TRANSFORM_BM8x8
 *
 * \param src Pointer to source image.
 *
 * \note
 * The destination VRAM address must be set in advance with
 * vmem_set_write_address().
 */
void im_adtr_v_BM8x8(const BM8x8 * src);

/**
 * `MSX` Rotate an 8x8 pixels 1bpp image 90 degrees clockwise.
 *
 * \ingroup IM_TRANSFORM_BM8x8
 *
 * \param src Pointer to source image.
 * \param dst Pointer to destination image.
 */
void im_rot90_BM8x8(const BM8x8 * src, BM8x8 * dst);

/**
 * `MSX` Rotate an 8x8 pixels 1bpp image 90 degrees clockwise.
 *
 * \ingroup IM_TRANSFORM_BM8x8
 *
 * \param src Pointer to source image.
 *
 * \note
 * The destination VRAM address must be set in advance with
 * vmem_set_write_address().
 */
void im_rot90_v_BM8x8(const BM8x8 * src);

/**
 * `MSX` Rotate an 8x8 pixels 1bpp image 180 degrees.
 *
 * \ingroup IM_TRANSFORM_BM8x8
 *
 * \param src Pointer to source image.
 * \param dst Pointer to destination image.
 */
void im_rot180_BM8x8(const BM8x8 * src, BM8x8 * dst);

/**
 * `MSX` Rotate an 8x8 pixels 1bpp image 180 degrees.
 *
 * \ingroup IM_TRANSFORM_BM8x8
 *
 * \param src Pointer to source image.
 *
 * \note
 * The destination VRAM address must be set in advance with
 * vmem_set_write_address().
 */
void im_rot180_v_BM8x8(const BM8x8 * src);

/**
 * `MSX` Rotate an 8x8 pixels 1bpp image 270 degrees clockwise.
 *
 * \ingroup IM_TRANSFORM_BM8x8
 *
 * \param src Pointer to source image.
 * \param dst Pointer to destination image.
 */
void im_rot270_BM8x8(const BM8x8 * src, BM8x8 * dst);

/**
 * `MSX` Rotate an 8x8 pixels 1bpp image 270 degrees clockwise.
 *
 * \ingroup IM_TRANSFORM_BM8x8
 *
 * \param src Pointer to source image.
 *
 * \note
 * The destination VRAM address must be set in advance with
 * vmem_set_write_address().
 */
void im_rot270_v_BM8x8(const BM8x8 * src);

/**
 * `MSX` Bitwise NOT (complement) of an 8x8 pixel 1bpp image.
 *
 * \ingroup IM_BOOLEAN_BM8x8
 *
 * \param src  Pointer to source image.
 * \param dst  Pointer to destination image.
 */
void im_cmpl_BM8x8(const BM8x8 * src, BM8x8 * dst);

/**
 * `MSX` Bitwise NOT (complement) of an 8x8 pixel 1bpp image.
 *
 * \ingroup IM_BOOLEAN_BM8x8
 *
 * \param src  Pointer to source image.
 *
 * \note
 * The destination VRAM address must be set in advance with
 * vmem_set_write_address().
 */
void im_cmpl_v_BM8x8(const BM8x8 * src);

/**
 * `MSX` Bitwise SET-DIFFERENCE of two 8x8 pixel 1bpp images.
 *
 * \ingroup IM_BOOLEAN_BM8x8
 *
 * \param src1 Pointer to source image.
 * \param src2 Pointer to another source image.
 * \param dst  Pointer to destination image.
 */
void im_diff_BM8x8(const BM8x8 * src1, const BM8x8 * src2, BM8x8 * dst);

/**
 * `MSX` Bitwise SET-DIFFERENCE of two 8x8 pixel 1bpp images.
 *
 * \ingroup IM_BOOLEAN_BM8x8
 *
 * \param src1 Pointer to source image.
 * \param src2 Pointer to another source image.
 *
 * \note
 * The destination VRAM address must be set in advance with
 * vmem_set_write_address().
 */
void im_diff_v_BM8x8(const BM8x8 * src1, const BM8x8 * src2);

/**
 * `MSX` Bitwise AND of two 8x8 pixel 1bpp images.
 *
 * \ingroup IM_BOOLEAN_BM8x8
 *
 * \param src1 Pointer to source image.
 * \param src2 Pointer to another source image.
 * \param dst  Pointer to destination image.
 */
void im_and_BM8x8(const BM8x8 * src1, const BM8x8 * src2, BM8x8 * dst);

/**
 * `MSX` Bitwise AND of two 8x8 pixel 1bpp images.
 *
 * \ingroup IM_BOOLEAN_BM8x8
 *
 * \param src1 Pointer to source image.
 * \param src2 Pointer to another source image.
 *
 * \note
 * The destination VRAM address must be set in advance with
 * vmem_set_write_address().
 */
void im_and_v_BM8x8(const BM8x8 * src1, const BM8x8 * src2);

/**
 * `MSX` Bitwise OR of two 8x8 pixel 1bpp images.
 *
 * \ingroup IM_BOOLEAN_BM8x8
 *
 * \param src1 Pointer to source image.
 * \param src2 Pointer to another source image.
 * \param dst  Pointer to destination image.
 */
void im_or_BM8x8(const BM8x8 * src1, const BM8x8 * src2, BM8x8 * dst);

/**
 * `MSX` Bitwise OR of two 8x8 pixel 1bpp images.
 *
 * \ingroup IM_BOOLEAN_BM8x8
 *
 * \param src1 Pointer to source image.
 * \param src2 Pointer to another source image.
 *
 * \note
 * The destination VRAM address must be set in advance with
 * vmem_set_write_address().
 */
void im_or_v_BM8x8(const BM8x8 * src1, const BM8x8 * src2);

/**
 * `MSX` Bitwise XOR of two 8x8 pixel 1bpp images.
 *
 * \ingroup IM_BOOLEAN_BM8x8
 *
 * \param src1 Pointer to source image.
 * \param src2 Pointer to another source image.
 * \param dst  Pointer to destination image.
 */
void im_xor_BM8x8(const BM8x8 * src1, const BM8x8 * src2, BM8x8 * dst);

/**
 * `MSX` Bitwise XOR of two 8x8 pixel 1bpp images.
 *
 * \ingroup IM_BOOLEAN_BM8x8
 *
 * \param src1 Pointer to source image.
 * \param src2 Pointer to another source image.
 *
 * \note
 * The destination VRAM address must be set in advance with
 * vmem_set_write_address().
 */
void im_xor_v_BM8x8(const BM8x8 * src1, const BM8x8 * src2);

/**
 * `MSX` Shift an 8x8 pixel 1bpp image upward.
 *
 * \ingroup IM_SHIFT_BM8x8
 *
 * \param sft Amount of shift in pixels.
 * \param src Pointer to source image.
 * \param dst Pointer to destination image.
 */
void im_shift_u_BM8x8(uint8_t sft, const BM8x8 * src, BM8x8 * dst);

/**
 * `MSX` Shift an 8x8 pixel 1bpp image upward.
 *
 * \ingroup IM_SHIFT_BM8x8
 *
 * \param sft Amount of shift in pixels.
 * \param src Pointer to source image.
 *
 * \note
 * The destination VRAM address must be set in advance with
 * vmem_set_write_address().
 */
void im_shift_u_v_BM8x8(uint8_t sft, const BM8x8 * src);

/**
 * `MSX` Shift an 8x8 pixel 1bpp image downward.
 *
 * \ingroup IM_SHIFT_BM8x8
 *
 * \param sft Amount of shift in pixels.
 * \param src Pointer to source image.
 * \param dst Pointer to destination image.
 */
void im_shift_d_BM8x8(uint8_t sft, const BM8x8 * src, BM8x8 * dst);

/**
 * `MSX` Shift an 8x8 pixel 1bpp image downward.
 *
 * \ingroup IM_SHIFT_BM8x8
 *
 * \param sft Amount of shift in pixels.
 * \param src Pointer to source image.
 *
 * \note
 * The destination VRAM address must be set in advance with
 * vmem_set_write_address().
 */
void im_shift_d_v_BM8x8(uint8_t sft, const BM8x8 * src);

/**
 * `MSX` Shift an 8x8 pixel 1bpp image to the left.
 *
 * \ingroup IM_SHIFT_BM8x8
 *
 * \param sft Amount of shift in pixels.
 * \param src Pointer to source image.
 * \param dst Pointer to destination image.
 */
void im_shift_l_BM8x8(uint8_t sft, const BM8x8 * src, BM8x8 * dst);

/**
 * `MSX` Shift an 8x8 pixel 1bpp image to the left.
 *
 * \ingroup IM_SHIFT_BM8x8
 *
 * \param sft Amount of shift in pixels.
 * \param src Pointer to source image.
 *
 * \note
 * The destination VRAM address must be set in advance with
 * vmem_set_write_address().
 */
void im_shift_l_v_BM8x8(uint8_t sft, const BM8x8 * src);

/**
 * `MSX` Shift an 8x8 pixel 1bpp image to the right.
 *
 * \ingroup IM_SHIFT_BM8x8
 *
 * \param sft Amount of shift in pixels.
 * \param src Pointer to source image.
 * \param dst Pointer to destination image.
 */
void im_shift_r_BM8x8(uint8_t sft, const BM8x8 * src, BM8x8 * dst);

/**
 * `MSX` Shift an 8x8 pixel 1bpp image to the right.
 *
 * \ingroup IM_SHIFT_BM8x8
 *
 * \param sft Amount of shift in pixels.
 * \param src Pointer to source image.
 *
 * \note
 * The destination VRAM address must be set in advance with
 * vmem_set_write_address().
 */
void im_shift_r_v_BM8x8(uint8_t sft, const BM8x8 * src);

/**
 * `MSX` Rotate (scroll) an 8x8 pixel 1bpp image upward.
 *
 * \ingroup IM_SHIFT_BM8x8
 *
 * \param sft Amount of rotation in pixels.
 * \param src Pointer to source image.
 * \param dst Pointer to destination image.
 */
void im_rotate_u_BM8x8(uint8_t sft, const BM8x8 * src, BM8x8 * dst);

/**
 * `MSX` Rotate (scroll) an 8x8 pixel 1bpp image upward.
 *
 * \ingroup IM_SHIFT_BM8x8
 *
 * \param sft Amount of rotation in pixels.
 * \param src Pointer to source image.
 *
 * \note
 * The destination VRAM address must be set in advance with
 * vmem_set_write_address().
 */
void im_rotate_u_v_BM8x8(uint8_t sft, const BM8x8 * src);

/**
 * `MSX` Rotate (scroll) an 8x8 pixel 1bpp image downward.
 *
 * \ingroup IM_SHIFT_BM8x8
 *
 * \param sft Amount of rotation in pixels.
 * \param src Pointer to source image.
 * \param dst Pointer to destination image.
 */
void im_rotate_d_BM8x8(uint8_t sft, const BM8x8 * src, BM8x8 * dst);

/**
 * `MSX` Rotate (scroll) an 8x8 pixel 1bpp image downward.
 *
 * \ingroup IM_SHIFT_BM8x8
 *
 * \param sft Amount of rotation in pixels.
 * \param src Pointer to source image.
 *
 * \note
 * The destination VRAM address must be set in advance with
 * vmem_set_write_address().
 */
void im_rotate_d_v_BM8x8(uint8_t sft, const BM8x8 * src);

/**
 * `MSX` Rotate (scroll) an 8x8 pixel 1bpp image to the left.
 *
 * \ingroup IM_SHIFT_BM8x8
 *
 * \param sft Amount of rotation in pixels.
 * \param src Pointer to source image.
 * \param dst Pointer to destination image.
 */
void im_rotate_l_BM8x8(uint8_t sft, const BM8x8 * src, BM8x8 * dst);

/**
 * `MSX` Rotate (scroll) an 8x8 pixel 1bpp image to the left.
 *
 * \ingroup IM_SHIFT_BM8x8
 *
 * \param sft Amount of rotation in pixels.
 * \param src Pointer to source image.
 *
 * \note
 * The destination VRAM address must be set in advance with
 * vmem_set_write_address().
 */
void im_rotate_l_v_BM8x8(uint8_t sft, const BM8x8 * src);

/**
 * `MSX` Rotate (scroll) an 8x8 pixel 1bpp image to the right.
 *
 * \ingroup IM_SHIFT_BM8x8
 *
 * \param sft Amount of rotation in pixels.
 * \param src Pointer to source image.
 * \param dst Pointer to destination image.
 */
void im_rotate_r_BM8x8(uint8_t sft, const BM8x8 * src, BM8x8 * dst);

/**
 * `MSX` Rotate (scroll) an 8x8 pixel 1bpp image to the right.
 *
 * \ingroup IM_SHIFT_BM8x8
 *
 * \param sft Amount of rotation in pixels.
 * \param src Pointer to source image.
 *
 * \note
 * The destination VRAM address must be set in advance with
 * vmem_set_write_address().
 */
void im_rotate_r_v_BM8x8(uint8_t sft, const BM8x8 * src);

/**
 * `MSX` Shift n-columns row vector of 8x8 pixel 1bpp image to the left.
 *
 * \ingroup IM_SHIFT_BM8x8
 *
 * \param sft Amount of shift in pixels. (0..7)
 * \param n   Number of columns. (n > 0)
 * \param src Pointer to n-element source images.
 * \param dst Pointer to n-element destination images.
 */
void im_shift_l_vec_BM8x8(uint8_t sft, size_t n, const BM8x8 * src, BM8x8 * dst);

/**
 * `MSX` Shift n-columns row vector of 8x8 pixel 1bpp image to the right.
 *
 * \ingroup IM_SHIFT_BM8x8
 *
 * \param sft Amount of shift in pixels. (0..7)
 * \param n   Number of columns. (n > 0)
 * \param src Pointer to n-element source images.
 * \param dst Pointer to n-element destination images.
 */
void im_shift_r_vec_BM8x8(uint8_t sft, size_t n, const BM8x8 * src, BM8x8 * dst);

/**
 * `MSX` Rotate (scroll) n-columns row vector of 8x8 pixel 1bpp image to the left.
 *
 * \ingroup IM_SHIFT_BM8x8
 *
 * \param sft Amount of rotation in pixels. (0..7)
 * \param n   Number of columns. (n > 0)
 * \param src Pointer to n-element source images.
 * \param dst Pointer to n-element destination images.
 */
void im_rotate_l_vec_BM8x8(uint8_t sft, size_t n, const BM8x8 * src, BM8x8 * dst);

/**
 * `MSX` Rotate (scroll) n-columns row vector of 8x8 pixel 1bpp image to the right.
 *
 * \ingroup IM_SHIFT_BM8x8
 *
 * \param sft Amount of rotation in pixels. (0..7)
 * \param n   Number of columns. (n > 0)
 * \param src Pointer to n-element source images.
 * \param dst Pointer to n-element destination images.
 */
void im_rotate_r_vec_BM8x8(uint8_t sft, size_t n, const BM8x8 * src, BM8x8 * dst);

/**
 * Enumeration of unary operators for images.
 *
 * \ingroup IM_TILEMAP_BM8x8
 *
 * These can be specified as operator identifier of tilemap element.
 *
 * \sa im_tilemap_BM8x8(), im_tilemap_v_BM8x8()
 */
enum ImOp {
  // Unary operators : image -> image
  /**
   * Identity transformation.
   * Corresponds to im_copy_BM8x8(), im_copy_v_BM8x8().
   */
  IM_IDENTITY      = 0,
  /**
   * Synonym for IM_IDENTITY.
   */
  IM_COPY          = IM_IDENTITY,
  /**
   * Bitwise NOT (complement).
   * Corresponds to im_cmpl_BM8x8(), im_cmpl_v_BM8x8().
   */
  IM_COMPLEMENT    = 1,
  /**
   * Transposition around the main diagonal.
   * Corresponds to im_tr_BM8x8(), im_tr_v_BM8x8().
   */
  IM_TRANSPOSE     = (1 << 5),
  /**
   * Flip vertically.
   * Corresponds to im_vflip_BM8x8(), im_vflip_v_BM8x8().
   */
  IM_FLIP_VERT     = (1 << 6),
  /**
   * Flip horizontally.
   * Corresponds to im_hflip_BM8x8(), im_hflip_v_BM8x8().
   */
  IM_FLIP_HORZ     = (1 << 7),
  /**
   * Rotate 90 degrees clockwise.
   * Corresponds to im_rot90_BM8x8(), im_rot90_v_BM8x8().
   */
  IM_ROTATE_90     = (IM_TRANSPOSE | IM_FLIP_HORZ),
  /**
   * Rotate 180 degrees.
   * Corresponds to im_rot180_BM8x8(), im_rot180_v_BM8x8().
   */
  IM_ROTATE_180    = (IM_FLIP_VERT | IM_FLIP_HORZ),
  /**
   * Rotate 270 degrees clockwise.
   * Corresponds to im_rot270_BM8x8(), im_rot270_v_BM8x8().
   */
  IM_ROTATE_270    = (IM_TRANSPOSE | IM_FLIP_VERT),
  /**
   * Transposition around the **antidiagonal**.
   * Corresponds to im_adtr_BM8x8(), im_adtr_v_BM8x8().
   */
  IM_ANTITRANSPOSE = (IM_TRANSPOSE | IM_FLIP_VERT | IM_FLIP_HORZ),
};

// *** NOT SUPPORTED YET ***
// /*
//  * Enumeration of boolean operators for images.
//  */
// enum ImOp_boolean {
//   // Unary operators : image -> image
//   IM_NOT             = IM_COMPLEMENT,
//   // Binary operators : (image, image) -> image
//   IM_AND             = 2,
//   IM_OR              = 4,
//   IM_XOR             = 6,
//   IM_NAND            = IM_NOT | IM_AND,
//   IM_NOR             = IM_NOT | IM_OR,
//   IM_NXOR            = IM_NOT | IM_XOR,
//   IM_INTERSECTION    = IM_AND,
//   IM_UNION           = IM_OR,
//   IM_SETDIFF         = 8,
//   // (reserved 9 to 15)
// };

// *** NOT SUPPORTED YET ***
// /*
//  * Enumeration of shift operators for images.
//  */
// enum ImOp_shift {
//   // Shift operators : (scalar, image) -> image
//   IM_SHIFT_UP        = 16,
//   IM_SHIFT_DOWN      = 17,
//   IM_SHIFT_LEFT      = 18,
//   IM_SHIFT_RIGHT     = 19,
//   IM_ROTATE_UP       = 20,
//   IM_ROTATE_DOWN     = 21,
//   IM_ROTATE_LEFT     = 22,
//   IM_ROTATE_RIGHT    = 23,
//   // (reserved 24 to 31)
// };

/**
 * `MSX` Construct a set of 8x8 pixels 1bpp tile images from tileset and
 * tilemap.
 *
 * \ingroup IM_TILEMAP_BM8x8
 *
 * **Tile**, **Tileset**, and **Tilemap** are similar to **Pixel**, **Color
 * palette**, and **Indexed color image** respectively.
 *
 * An indexed color image is an array/matrix of pixels. The representation of
 * each pixel (i.e. color) is specified by its index into a color palette.
 *
 * A tilemap image, on the other hand, is an array/matrix of tiles. The
 * representation of each tile is usually specified by the tileset index (and
 * optional attributes).
 *
 * The concepts of **Tile**, **Tileset**, and **Tilemap** in this function are
 * defined as follows:
 *
 * - **Tile**    : Small fixed size image. (8x8px 1bpp image).
 * - **Tileset** : A set of tile images. (256 8x8px 1bpp images).
 * - **Tilemap** : An array of 2-byte cells.
 *                 - The first byte of the cell is the index of the tileset, and
 *                 - The second byte is the identifier of the operator applied
 *                   to the tile (one of `enum ImOp`).
 *
 * The below pseudo code shows what the function performs:
 *
 * ~~~ c
 * for (size_t i = 0; i < n; ++i) {
 *   uint8_t idx = tilemap[i][0];
 *   uint8_t op = tilemap[i][1];
 *   switch (op) {
 *     case IM_COPY: im_copy_BM8x8(&tileset[idx], &dst[i]); break;
 *     case ...
 *   }
 * }
 * ~~~
 *
 * \param tileset Pointer to source tileset (256 sets of 8x8px 1bpp images).
 * \param n       Number of elements of the `tilemap`.
 * \param tilemap Pointer to `n` element array of 2-byte cells.
 *                - The first byte of the cell is the index of the tileset, and
 *                - The second byte is the identifier of the operator applied to
 *                  the tile (one of `enum ImOp`).
 * \param dst     Pointer to destination `n` elements array of 8x8px 1bpp images.
 *
 * \sa enum ImOp
 * \sa im_tilemap_v_BM8x8()
 */
void im_tilemap_BM8x8(const BM8x8 tileset[256], size_t n, const uint8_t tilemap[][2], BM8x8 * dst);

/**
 * `MSX` Construct a set of 8x8 pixels 1bpp tile images from tileset and
 * tilemap.
 *
 * \ingroup IM_TILEMAP_BM8x8
 *
 * A variant of im_tilemap_BM8x8(). (writes output to VRAM instead of RAM).
 *
 * The below pseudo code shows what the function performs:
 *
 * ~~~ c
 * for (size_t i = 0; i < n; ++i) {
 *   uint8_t idx = tilemap[i][0];
 *   uint8_t op = tilemap[i][1];
 *   switch (op) {
 *     case IM_COPY: im_copy_v_BM8x8(&tileset[idx]); break;
 *     case ...
 *   }
 * }
 * ~~~
 *
 * \param tileset Pointer to source tileset (256 sets of 8x8px 1bpp images).
 * \param n       Number of elements of the `tilemap`.
 * \param tilemap Pointer to `n` element array of 2-byte cells.
 *                - The first byte of the cell is the index of the tileset, and
 *                - The second byte is the identifier of the operator applied to
 *                  the tile (one of `enum ImOp`).
 *
 * \sa enum ImOp
 * \sa im_tilemap_BM8x8()
 *
 * \note
 * The destination VRAM address must be set in advance with
 * vmem_set_write_address().
 */
void im_tilemap_v_BM8x8(const BM8x8 tileset[256], size_t n, const uint8_t tilemap[][2]);

/**
 * `MSX` Copy an 16x16 pixels 1bpp sprite pattern.
 *
 * \ingroup IM_TRANSFORM_SP16x16
 *
 * \param src Pointer to source image.
 * \param dst Pointer to destination image.
 */
void im_copy_SP16x16(const SP16x16 * src, SP16x16 * dst);

/**
 * `MSX` Copy an 16x16 pixels 1bpp sprite pattern.
 *
 * \ingroup IM_TRANSFORM_SP16x16
 *
 * \param src Pointer to source image.
 *
 * \note
 * The destination VRAM address must be set in advance with
 * vmem_set_write_address().
 */
void im_copy_v_SP16x16(const SP16x16 * src);

/**
 * `MSX` Flip an 16x16 pixels 1bpp sprite pattern horizontally.
 * \ingroup IM_TRANSFORM_SP16x16
 *
 * \param src Pointer to source image.
 * \param dst Pointer to destination image.
 */
void im_hflip_SP16x16(const SP16x16 * src, SP16x16 * dst);

/**
 * `MSX` Flip an 16x16 pixels 1bpp sprite pattern horizontally.
 * \ingroup IM_TRANSFORM_SP16x16
 *
 * \param src Pointer to source image.
 *
 * \note
 * The destination VRAM address must be set in advance with
 * vmem_set_write_address().
 */
void im_hflip_v_SP16x16(const SP16x16 * src);

/**
 * `MSX` Flip an 16x16 pixels 1bpp sprite pattern vertically.
 * \ingroup IM_TRANSFORM_SP16x16
 *
 * \param src Pointer to source image.
 * \param dst Pointer to destination image.
 */
void im_vflip_SP16x16(const SP16x16 * src, SP16x16 * dst);

/**
 * `MSX` Flip an 16x16 pixels 1bpp sprite pattern vertically.
 * \ingroup IM_TRANSFORM_SP16x16
 *
 * \param src Pointer to source image.
 *
 * \note
 * The destination VRAM address must be set in advance with
 * vmem_set_write_address().
 */
void im_vflip_v_SP16x16(const SP16x16 * src);

/**
 * `MSX` Transpose an 16x16 pixels 1bpp sprite pattern around the main diagonal.
 *
 * \ingroup IM_TRANSFORM_SP16x16
 *
 * \param src Pointer to source image.
 * \param dst Pointer to destination image.
 */
void im_tr_SP16x16(const SP16x16 * src, SP16x16 * dst);

/**
 * `MSX` Transpose an 16x16 pixels 1bpp sprite pattern around the main diagonal.
 *
 * \ingroup IM_TRANSFORM_SP16x16
 *
 * \param src Pointer to source image.
 *
 * \note
 * The destination VRAM address must be set in advance with
 * vmem_set_write_address().
 */
void im_tr_v_SP16x16(const SP16x16 * src);

/**
 * `MSX` Transpose an 16x16 pixels 1bpp sprite pattern around the
 * **antidiagonal**.
 *
 * \ingroup IM_TRANSFORM_SP16x16
 *
 * \param src Pointer to source image.
 * \param dst Pointer to destination image.
 */
void im_adtr_SP16x16(const SP16x16 * src, SP16x16 * dst);

/**
 * `MSX` Transpose an 16x16 pixels 1bpp sprite pattern around the
 * **antidiagonal**.
 *
 * \ingroup IM_TRANSFORM_SP16x16
 *
 * \param src Pointer to source image.
 *
 * \note
 * The destination VRAM address must be set in advance with
 * vmem_set_write_address().
 */
void im_adtr_v_SP16x16(const SP16x16 * src);

/**
 * `MSX` Rotate an 16x16 pixels 1bpp sprite pattern 90 degrees clockwise.
 *
 * \ingroup IM_TRANSFORM_SP16x16
 *
 * \param src Pointer to source image.
 * \param dst Pointer to destination image.
 */
void im_rot90_SP16x16(const SP16x16 * src, SP16x16 * dst);

/**
 * `MSX` Rotate an 16x16 pixels 1bpp sprite pattern 90 degrees clockwise.
 *
 * \ingroup IM_TRANSFORM_SP16x16
 *
 * \param src Pointer to source image.
 *
 * \note
 * The destination VRAM address must be set in advance with
 * vmem_set_write_address().
 */
void im_rot90_v_SP16x16(const SP16x16 * src);

/**
 * `MSX` Rotate an 16x16 pixels 1bpp sprite pattern 180 degrees.
 *
 * \ingroup IM_TRANSFORM_SP16x16
 *
 * \param src Pointer to source image.
 * \param dst Pointer to destination image.
 */
void im_rot180_SP16x16(const SP16x16 * src, SP16x16 * dst);

/**
 * `MSX` Rotate an 16x16 pixels 1bpp sprite pattern 180 degrees.
 *
 * \ingroup IM_TRANSFORM_SP16x16
 *
 * \param src Pointer to source image.
 *
 * \note
 * The destination VRAM address must be set in advance with
 * vmem_set_write_address().
 */
void im_rot180_v_SP16x16(const SP16x16 * src);

/**
 * `MSX` Rotate an 16x16 pixels 1bpp sprite pattern 270 degrees clockwise.
 *
 * \ingroup IM_TRANSFORM_SP16x16
 *
 * \param src Pointer to source image.
 * \param dst Pointer to destination image.
 */
void im_rot270_SP16x16(const SP16x16 * src, SP16x16 * dst);

/**
 * `MSX` Rotate an 16x16 pixels 1bpp sprite pattern 270 degrees clockwise.
 *
 * \ingroup IM_TRANSFORM_SP16x16
 *
 * \param src Pointer to source image.
 *
 * \note
 * The destination VRAM address must be set in advance with
 * vmem_set_write_address().
 */
void im_rot270_v_SP16x16(const SP16x16 * src);

/**
 * `MSX` Bitwise NOT (complement) of an 16x16 pixels 1bpp sprite pattern.
 *
 * \ingroup IM_BOOLEAN_SP16x16
 *
 * \param src  Pointer to source image.
 * \param dst  Pointer to destination image.
 */
void im_cmpl_SP16x16(const SP16x16 * src, SP16x16 * dst);

/**
 * `MSX` Bitwise NOT (complement) of an 16x16 pixels 1bpp sprite pattern.
 *
 * \ingroup IM_BOOLEAN_SP16x16
 *
 * \param src  Pointer to source image.
 *
 * \note
 * The destination VRAM address must be set in advance with
 * vmem_set_write_address().
 */
void im_cmpl_v_SP16x16(const SP16x16 * src);

/**
 * `MSX` Bitwise SET-DIFFERENCE of two 16x16 pixels 1bpp sprite patterns.
 *
 * \ingroup IM_BOOLEAN_SP16x16
 *
 * \param src1 Pointer to source image.
 * \param src2 Pointer to another source image.
 * \param dst  Pointer to destination image.
 */
void im_diff_SP16x16(const SP16x16 * src1, const SP16x16 * src2, SP16x16 * dst);

/**
 * `MSX` Bitwise SET-DIFFERENCE of two 16x16 pixels 1bpp sprite patterns.
 *
 * \ingroup IM_BOOLEAN_SP16x16
 *
 * \param src1 Pointer to source image.
 * \param src2 Pointer to another source image.
 *
 * \note
 * The destination VRAM address must be set in advance with
 * vmem_set_write_address().
 */
void im_diff_v_SP16x16(const SP16x16 * src1, const SP16x16 * src2);

/**
 * `MSX` Bitwise AND of two 16x16 pixel 1bpp sprite patterns.
 *
 * \ingroup IM_BOOLEAN_SP16x16
 *
 * \param src1 Pointer to source image.
 * \param src2 Pointer to another source image.
 * \param dst  Pointer to destination image.
 */
void im_and_SP16x16(const SP16x16 * src1, const SP16x16 * src2, SP16x16 * dst);

/**
 * `MSX` Bitwise AND of two 16x16 pixel 1bpp sprite patterns.
 *
 * \ingroup IM_BOOLEAN_SP16x16
 *
 * \param src1 Pointer to source image.
 * \param src2 Pointer to another source image.
 *
 * \note
 * The destination VRAM address must be set in advance with
 * vmem_set_write_address().
 */
void im_and_v_SP16x16(const SP16x16 * src1, const SP16x16 * src2);

/**
 * `MSX` Bitwise OR of two 16x16 pixel 1bpp sprite patterns.
 *
 * \ingroup IM_BOOLEAN_SP16x16
 *
 * \param src1 Pointer to source image.
 * \param src2 Pointer to another source image.
 * \param dst  Pointer to destination image.
 */
void im_or_SP16x16(const SP16x16 * src1, const SP16x16 * src2, SP16x16 * dst);

/**
 * `MSX` Bitwise OR of two 16x16 pixel 1bpp sprite patterns.
 *
 * \ingroup IM_BOOLEAN_SP16x16
 *
 * \param src1 Pointer to source image.
 * \param src2 Pointer to another source image.
 *
 * \note
 * The destination VRAM address must be set in advance with
 * vmem_set_write_address().
 */
void im_or_v_SP16x16(const SP16x16 * src1, const SP16x16 * src2);

/**
 * `MSX` Bitwise XOR of two 16x16 pixel 1bpp sprite patterns.
 *
 * \ingroup IM_BOOLEAN_SP16x16
 *
 * \param src1 Pointer to source image.
 * \param src2 Pointer to another source image.
 * \param dst  Pointer to destination image.
 */
void im_xor_SP16x16(const SP16x16 * src1, const SP16x16 * src2, SP16x16 * dst);

/**
 * `MSX` Bitwise XOR of two 16x16 pixel 1bpp sprite patterns.
 *
 * \ingroup IM_BOOLEAN_SP16x16
 *
 * \param src1 Pointer to source image.
 * \param src2 Pointer to another source image.
 *
 * \note
 * The destination VRAM address must be set in advance with
 * vmem_set_write_address().
 */
void im_xor_v_SP16x16(const SP16x16 * src1, const SP16x16 * src2);

/**
 * `MSX` Shift an 16x16 pixel 1bpp sprite pattern upward.
 *
 * \ingroup IM_SHIFT_SP16x16
 *
 * \param sft Amount of shift in pixels.
 * \param src Pointer to source image.
 * \param dst Pointer to destination image.
 */
void im_shift_u_SP16x16(uint8_t sft, const SP16x16 * src, SP16x16 * dst);

/**
 * `MSX` Shift an 16x16 pixel 1bpp sprite pattern upward.
 *
 * \ingroup IM_SHIFT_SP16x16
 *
 * \param sft Amount of shift in pixels.
 * \param src Pointer to source image.
 *
 * \note
 * The destination VRAM address must be set in advance with
 * vmem_set_write_address().
 */
void im_shift_u_v_SP16x16(uint8_t sft, const SP16x16 * src);

/**
 * `MSX` Shift an 16x16 pixel 1bpp sprite pattern downward.
 *
 * \ingroup IM_SHIFT_SP16x16
 *
 * \param sft Amount of shift in pixels.
 * \param src Pointer to source image.
 * \param dst Pointer to destination image.
 */
void im_shift_d_SP16x16(uint8_t sft, const SP16x16 * src, SP16x16 * dst);

/**
 * `MSX` Shift an 16x16 pixel 1bpp sprite pattern downward.
 *
 * \ingroup IM_SHIFT_SP16x16
 *
 * \param sft Amount of shift in pixels.
 * \param src Pointer to source image.
 *
 * \note
 * The destination VRAM address must be set in advance with
 * vmem_set_write_address().
 */
void im_shift_d_v_SP16x16(uint8_t sft, const SP16x16 * src);

/**
 * `MSX` Shift an 16x16 pixel 1bpp sprite pattern to the left.
 *
 * \ingroup IM_SHIFT_SP16x16
 *
 * \param sft Amount of shift in pixels.
 * \param src Pointer to source image.
 * \param dst Pointer to destination image.
 */
void im_shift_l_SP16x16(uint8_t sft, const SP16x16 * src, SP16x16 * dst);

/**
 * `MSX` Shift an 16x16 pixel 1bpp sprite pattern to the left.
 *
 * \ingroup IM_SHIFT_SP16x16
 *
 * \param sft Amount of shift in pixels.
 * \param src Pointer to source image.
 *
 * \note
 * The destination VRAM address must be set in advance with
 * vmem_set_write_address().
 */
void im_shift_l_v_SP16x16(uint8_t sft, const SP16x16 * src);

/**
 * `MSX` Shift an 16x16 pixel 1bpp sprite pattern to the right.
 *
 * \ingroup IM_SHIFT_SP16x16
 *
 * \param sft Amount of shift in pixels.
 * \param src Pointer to source image.
 * \param dst Pointer to destination image.
 */
void im_shift_r_SP16x16(uint8_t sft, const SP16x16 * src, SP16x16 * dst);

/**
 * `MSX` Shift an 16x16 pixel 1bpp sprite pattern to the right.
 *
 * \ingroup IM_SHIFT_SP16x16
 *
 * \param sft Amount of shift in pixels.
 * \param src Pointer to source image.
 *
 * \note
 * The destination VRAM address must be set in advance with
 * vmem_set_write_address().
 */
void im_shift_r_v_SP16x16(uint8_t sft, const SP16x16 * src);

/**
 * `MSX` Rotate (scroll) an 16x16 pixel 1bpp sprite pattern upward.
 *
 * \ingroup IM_SHIFT_SP16x16
 *
 * \param sft Amount of rotation in pixels.
 * \param src Pointer to source image.
 * \param dst Pointer to destination image.
 */
void im_rotate_u_SP16x16(uint8_t sft, const SP16x16 * src, SP16x16 * dst);

/**
 * `MSX` Rotate (scroll) an 16x16 pixel 1bpp sprite pattern upward.
 *
 * \ingroup IM_SHIFT_SP16x16
 *
 * \param sft Amount of rotation in pixels.
 * \param src Pointer to source image.
 *
 * \note
 * The destination VRAM address must be set in advance with
 * vmem_set_write_address().
 */
void im_rotate_u_v_SP16x16(uint8_t sft, const SP16x16 * src);

/**
 * `MSX` Rotate (scroll) an 16x16 pixel 1bpp sprite pattern downward.
 *
 * \ingroup IM_SHIFT_SP16x16
 *
 * \param sft Amount of rotation in pixels.
 * \param src Pointer to source image.
 * \param dst Pointer to destination image.
 */
void im_rotate_d_SP16x16(uint8_t sft, const SP16x16 * src, SP16x16 * dst);

/**
 * `MSX` Rotate (scroll) an 16x16 pixel 1bpp sprite pattern downward.
 *
 * \ingroup IM_SHIFT_SP16x16
 *
 * \param sft Amount of rotation in pixels.
 * \param src Pointer to source image.
 *
 * \note
 * The destination VRAM address must be set in advance with
 * vmem_set_write_address().
 */
void im_rotate_d_v_SP16x16(uint8_t sft, const SP16x16 * src);

/**
 * `MSX` Rotate (scroll) an 16x16 pixel 1bpp sprite pattern to the left.
 *
 * \ingroup IM_SHIFT_SP16x16
 *
 * \param sft Amount of rotation in pixels.
 * \param src Pointer to source image.
 * \param dst Pointer to destination image.
 */
void im_rotate_l_SP16x16(uint8_t sft, const SP16x16 * src, SP16x16 * dst);

/**
 * `MSX` Rotate (scroll) an 16x16 pixel 1bpp sprite pattern to the left.
 *
 * \ingroup IM_SHIFT_SP16x16
 *
 * \param sft Amount of rotation in pixels.
 * \param src Pointer to source image.
 *
 * \note
 * The destination VRAM address must be set in advance with
 * vmem_set_write_address().
 */
void im_rotate_l_v_SP16x16(uint8_t sft, const SP16x16 * src);

/**
 * `MSX` Rotate (scroll) an 16x16 pixel 1bpp sprite pattern to the right.
 *
 * \ingroup IM_SHIFT_SP16x16
 *
 * \param sft Amount of rotation in pixels.
 * \param src Pointer to source image.
 * \param dst Pointer to destination image.
 */
void im_rotate_r_SP16x16(uint8_t sft, const SP16x16 * src, SP16x16 * dst);

/**
 * `MSX` Rotate (scroll) an 16x16 pixel 1bpp sprite pattern to the right.
 *
 * \ingroup IM_SHIFT_SP16x16
 *
 * \param sft Amount of rotation in pixels.
 * \param src Pointer to source image.
 *
 * \note
 * The destination VRAM address must be set in advance with
 * vmem_set_write_address().
 */
void im_rotate_r_v_SP16x16(uint8_t sft, const SP16x16 * src);

/**
 * `MSX` Trim 16x16 pixels 1bpp sprite pattern.
 *
 * \ingroup IM_TRIM_SP16x16
 *
 * \param tblr Amount of pixels to be trimmed.\n
 *             1 byte each for top, bottom, left, and right.
 *             - tblr[0] : Top
 *             - tblr[1] : Bottom
 *             - tblr[2] : Left
 *             - tblr[3] : Right
 * \param src  Pointer to source sprite pattern.
 * \param src  Pointer to destination sprite pattern.
 */
void im_trim_SP16x16(uint8_t tblr[4], const SP16x16 * src, SP16x16 * dst);

#endif // IMG_H_
