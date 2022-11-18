;;; -*- mode: asm; coding: utf-8-unix; tab-width: 8 -*-

;;; \file crt0/rom_mapper/rom_ascii8.s
;;;
;;; Copyright (c) 2021-2022 Daishi Mori (mori0091)
;;;
;;; This software is released under the MIT License.
;;; See https://github.com/mori0091/libmsx/blob/main/LICENSE
;;;
;;; GitHub libmsx project
;;; https://github.com/mori0091/libmsx

        .module crt0

        ;; ROM Mapper registers (ROM type: ASCII8)
        ROM_MAPPER_REGISTER_0 = 0x6000
        ROM_MAPPER_REGISTER_1 = 0x6800
        ROM_MAPPER_REGISTER_2 = 0x7000
        ROM_MAPPER_REGISTER_3 = 0x7800

        .area   _CODE
rom_init::
        xor     a
        ld      (ROM_MAPPER_REGISTER_0), a ; segment #0
        inc     a
        ld      (ROM_MAPPER_REGISTER_1), a ; segment #1
        inc     a
        ld      (ROM_MAPPER_REGISTER_2), a ; segment #2 (bank #0.0)
        inc     a
        ld      (ROM_MAPPER_REGISTER_3), a ; segment #3 (bank #0.1)
        ret

;------------------------------------------------
set_bank::
        ld      (cur_bank), a
        ;; two 8KiB pages per one bank
        inc     a
        add     a, a
        ld      (ROM_MAPPER_REGISTER_2), a ; segment #2n+2 (lower 8KiB of bank #n)
        inc     a
        ld      (ROM_MAPPER_REGISTER_3), a ; segment #2n+3 (upper 8KiB of bank #n)
        ret

;------------------------------------------------
get_bank::
        ld      a, (cur_bank)
        ret

;------------------------------------------------
        .area   _DATA
cur_bank:
        .ds     1
