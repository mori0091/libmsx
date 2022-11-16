;;; -*- mode: asm; coding: utf-8-unix; tab-width: 8 -*-

;;; Copyright (c) 2021 Daishi Mori (mori0091)
;;;
;;; This software is released under the MIT License.
;;; See https://github.com/mori0091/libmsx/blob/main/LICENSE
;;;
;;; GitHub libmsx project
;;; https://github.com/mori0091/libmsx

;;; \file crt0/16k.4000/crt0x.s
;;;
;;; crt0 for MSX ROM of 16KB starting at 0x4000
;;; suggested options: --code-loc 0x4010 --data-loc 0xc000
;;; `main` should be `void main(void)`
;;; `return` from `main` causes soft reset.
;;;
;;; `main` starts after ALL cartridges are ready.
;;; Thus, for example, BDOS is available if FDD exists.

        .module crt0x
        .globl  _main
        .globl  _exit
        .globl  _libmsx___init_intr

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
        .area   _DATA
        .area   _INITIALIZED
        .area   _BSEG
        .area   _BSS
        .area   _HEAP
        ;; ----

        .area   _CODE
init:
        H_STKE = 0xfeda
        ld      hl, #boot
        ld      de, #H_STKE
        ld      bc, #5
        ldir
        call    get_slot_page1
        ld      (H_STKE+1), a
        ret
boot:   ;; template for H.STKE hook
        rst     0x30
        .db     1
        .dw     start
        ret
start:
        HIMEM  = 0xfc4a
        ld      sp,(HIMEM)
        call    gsinit
        call    _libmsx___init_intr
        call    _main
_exit::
        rst     0x00

;------------------------------------------------
        .include        "../../crtlib/get_slot_page1.s"
        .include        "../../sdcc/device/lib/z80/gsinit.s"
