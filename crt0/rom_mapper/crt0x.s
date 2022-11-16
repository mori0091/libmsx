;;; -*- mode: asm; coding: utf-8-unix; tab-width: 8 -*-

;;; Copyright (c) 2022 Daishi Mori (mori0091)
;;;
;;; This software is released under the MIT License.
;;; See https://github.com/mori0091/libmsx/blob/main/LICENSE
;;;
;;; GitHub libmsx project
;;; https://github.com/mori0091/libmsx

;;; \file crt0/rom_mapper/crt0.s
;;; C startup routine for MSX w/ ROM mapper and SDCC banked call support.
;;;
;;; crt0 for MSX ROM of 32KB starting at 0x4000
;;; suggested options: --code-loc 0x4010 --data-loc 0xc000
;;; `main` should be `void main(void)`
;;; `return` from `main` causes soft reset.
;;;
;;; `main` starts after ALL cartridges are ready.
;;; Thus, for example, BDOS is available if FDD exists.
;;;
;;; \note
;;; It is a common part of crt0 and is intended to be used with one of a ROM
;;; mapper-specific modules such as rom_ascii8.s and rom_ascii16.s.

        .module crt0x

        .globl  _main
        .globl  _exit
        .globl  _libmsx___init_intr
        .globl rom_init
        .globl set_bank

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
        ;; ----
        .area   _BANK0
        ;; ----
        .area   _DATA
        .area   _INITIALIZED
        .area   _BSEG
        .area   _BSS
        .area   _HEAP
        ;; ----
        .area   _BANK1
        .area   _BANK2
        .area   _BANK3
        .area   _BANK4
        .area   _BANK5
        .area   _BANK6
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
        HIMEM  = 0xfc4a         ; (2 bytes) Pointer to upper limit address of free area
        ld      sp,(HIMEM)
        call    get_slot_page1
        call    set_slot_page2
        call    rom_init        ; select ROM banks
        call    gsinit          ; initialize RAM
        call    _libmsx___init_intr ; install interrupt routine
        call    _main
_exit::
        rst     0x00

;------------------------------------------------
        .include        "../../crtlib/get_slot_page1.s"
        .include        "../../crtlib/set_slot_page2.s"
        .include        "../../sdcc/device/lib/z80/gsinit.s"
