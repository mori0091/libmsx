; -*- coding: utf-8-unix; tab-width: 8; mode: asm -*-

; --------------------------------------------------------------
;;; Copyright (c) 2021-2025 Daishi Mori (mori0091)
;;;
;;; This software is released under the MIT License.
;;; See https://github.com/mori0091/libmsx/blob/main/LICENSE
;;;
;;; GitHub libmsx project
;;; https://github.com/mori0091/libmsx
; --------------------------------------------------------------
;;; \file NDP_set_volume.s
; --------------------------------------------------------------

	.module	NDP_set_volume

	.area	_CODE

	.globl	_NDP_MVSET

_NDP_set_volume::		; void NDP_set_volume(uint8_t vol)
	xor	a, #0x0f
	and	a, #0x0f
	jp	_NDP_MVSET
