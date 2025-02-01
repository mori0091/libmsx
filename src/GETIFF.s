;;; -*- mode: asm; coding: utf-8-unix; tab-width: 8 -*-

;;; Copyright (c) 2021-2025 Daishi Mori (mori0091)
;;;
;;; This software is released under the MIT License.\n
;;; See https://github.com/mori0091/libmsx/blob/main/LICENSE
;;;
;;; GitHub libmsx project\n
;;; https://github.com/mori0091/libmsx

;;; \file GETIFF.s
;;; Get the correct state of the interrupts.
;;;
;;; \sa
;;; "Z80 FAMILY DATA BOOK", 1989, Zilog, pp.412-413.

        .module GETIFF

        .area   _CODE

;;; Get the correct state of the interrupts.
;;;
;;; \post `AF` register will be changed.
;;; \post C flag is set if interrupts are disabled.
;;;
;;; \note
;;; Listing 1: This routine may not be loaded in page zero
;;; (0000h to OOFFh).
GETIFF::
        xor     a               ; C flag, acc. := 0
        push    af              ; stack bottom := 00xxh
        pop     af              ; Restore SP
        ld      a, i            ; P flag := IFF2
        ret     pe              ; Exit if enabled
        dec     sp              ; May be disabled.
        dec     sp              ; Has stack bottom been
        pop     af              ; overwritten ?
        and     a               ; If not OOxxh, INTs were
        ret     nz              ; actually enabled.
        scf                     ; Otherwise, they really are
        ret                     ; disabled.
