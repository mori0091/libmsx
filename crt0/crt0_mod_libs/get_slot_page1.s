;;; -*- mode: asm; coding: utf-8-unix; tab-width: 8 -*-

;;; Copyright (c) 2021-2022 Daishi Mori (mori0091)
;;;
;;; This software is released under the MIT License.\n
;;; See https://github.com/mori0091/libmsx/blob/main/LICENSE
;;;
;;; GitHub libmsx project\n
;;; https://github.com/mori0091/libmsx

;;; \file get_slot_page1.s

        .module get_slot_page1

        .globl get_slot

;; \brief Get slot address of page #1.
;;
;; \retval A  slot
;; \post Interrupts are enabled.
;; \post AF, BC, HL are changed.
        .area   _CODE
get_slot_page1::
        ld      h, #0x40
        jp      get_slot
