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
;;; \file NDP_version.s
; --------------------------------------------------------------

	.module	NDP_version

	.area	_CODE

	.globl	_NDP_SYSVER

_NDP_version::			; uint16_t NDP_version(void)
	push	hl
	call	_NDP_SYSVER
	ex	de, hl
	pop	hl
	ret
