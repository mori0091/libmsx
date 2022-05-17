;;; -*- mode: asm; coding: utf-8-unix; tab-width: 8 -*-

;;; \file crt0/rom_mapper/rom_ascii16.s
;;;
;;; Copyright (c) 2022 Daishi Mori (mori0091)
;;;
;;; This software is released under the MIT License.
;;; See https://github.com/mori0091/libmsx/blob/main/LICENSE
;;;
;;; GitHub libmsx project
;;; https://github.com/mori0091/libmsx

        .module crt0

        ;; ROM Mapper registers (ROM type: ASCII16)
        ROM_MAPPER_REGISTER_0 = 0x6000
        ROM_MAPPER_REGISTER_1 = 0x7000

        .area   _CODE
rom_init::
        xor     a
        ld      (ROM_MAPPER_REGISTER_0), a ; segment #0
        inc     a
        ld      (ROM_MAPPER_REGISTER_1), a ; segment #1 (bank #0)
        ret

;------------------------------------------------
set_bank::
        ld      (cur_bank), a
        ;; one 16KiB page per one bank
        inc     a
        ld      (ROM_MAPPER_REGISTER_1), a ; segment #n+1 (bank #n)
        ret

;------------------------------------------------
get_bank::
        ld      a, (cur_bank)
        ret

;------------------------------------------------
        .area   _DATA
cur_bank:
        .ds     1
