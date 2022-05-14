;;; -*- mode: asm; coding: utf-8-unix; tab-width: 8 -*-

;;; \file crt0/16k.4000/crt0.s
;;;
;;; Copyright (c) 2021 Daishi Mori (mori0091)
;;;
;;; This software is released under the MIT License.
;;; See https://github.com/mori0091/libmsx/blob/main/LICENSE
;;;
;;; GitHub libmsx project
;;; https://github.com/mori0091/libmsx

;;; crt0 for MSX ROM of 16KB starting at 0x4000
;;; suggested options: --code-loc 0x4010 --data-loc 0xc000
;;; `main` should be `void main(void)`
;;; `return` from `main` causes soft reset.

        .module crt0
        .globl  _main
        .globl  _exit
        .globl  _libmsx___init_intr

        HIMEM  = 0xfc4a

        .area   _HEADER (ABS)
        ;; ROM header
        .org    0x4000

        .db     0x41
        .db     0x42
        .dw     init
        .dw     0x0000
        .dw     0x0000
        .dw     0x0000
        .dw     0x0000
        .dw     0x0000
        .dw     0x0000

        ;; ---- ordering segments for linker
        .area   _HOME
        .area   _CODE
        .area   _INITIALIZER
        .area   _GSINIT
	.area   _GSFINAL
	.area	_DATA
	.area	_INITIALIZED
	.area	_BSEG
	.area   _BSS
	.area   _HEAP
        ;; ----

	.area   _CODE
init:
        ld      sp,(HIMEM)
        call    gsinit
        call    _libmsx___init_intr
        call    _main
_exit::
        rst     0x00

;------------------------------------------------
        .include        "../../sdcc/device/lib/z80/gsinit.s"
