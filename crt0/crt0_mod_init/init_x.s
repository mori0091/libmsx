;;; -*- mode: asm; coding: utf-8-unix; tab-width: 8 -*-

;;; Copyright (c) 2021-2022 Daishi Mori (mori0091)
;;;
;;; This software is released under the MIT License.
;;; See https://github.com/mori0091/libmsx/blob/main/LICENSE
;;;
;;; GitHub libmsx project
;;; https://github.com/mori0091/libmsx

;;; \file crt0/init/init_x.s
;;; One of an `init` module for MSX ROM image.
;;;
;;; `init` of this starts `main()` after initialization phase of all system
;;; extension is done. Application starts on Z80 mode with MSX-DOS 1.x / Disk
;;; BASIC 1.x supoort in default.

        .module init_x

        .globl  start
        .globl  get_slot_page1

        H_STKE = 0xfeda

        .area   _CODE
init::
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
