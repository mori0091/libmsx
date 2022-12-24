;;; -*- mode: asm; coding: utf-8-unix; tab-width: 8 -*-

;;; Copyright (c) 2021-2022 Daishi Mori (mori0091)
;;;
;;; This software is released under the MIT License.\n
;;; See https://github.com/mori0091/libmsx/blob/main/LICENSE
;;;
;;; GitHub libmsx project\n
;;; https://github.com/mori0091/libmsx

;;; \file crt0/init/init_r.s
;;; One of an `init` module for MSX ROM image.
;;;
;;; `init` of this starts `main()` after initialization phase of all system
;;; extension is done. If the platform is `MSXturboR`, application starts on
;;; R800 DRAM mode with MSX-DOS 2.x / Disk BASIC 2.x supoort in default.

        .module init_r

        .globl  start
        .globl  get_slot

        H_STKE = 0xfeda
        USRTAB = 0xf39a

        .area   _CODE
init::
        ld      hl, #boot
        ld      de, #H_STKE
        ld      bc, #5
        ldir
        ;; get slot of page 1
        ld      h, #0x40
        call    get_slot
        ld      (H_STKE+1), a
        ;; --- Use R800 DRAM mode w/ MSX-DOS 2.x / Disk BASIC v2.x ---
        ld      hl, #0x6474
        ld      (USRTAB+0), hl
        ;; ---
        ret
boot:   ;; template for H.STKE hook
        rst     0x30
        .db     1
        .dw     start
        ret
