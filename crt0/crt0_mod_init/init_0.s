;;; -*- mode: asm; coding: utf-8-unix; tab-width: 8 -*-

;;; Copyright (c) 2021-2023 Daishi Mori (mori0091)
;;;
;;; This software is released under the MIT License.
;;; See https://github.com/mori0091/libmsx/blob/main/LICENSE
;;;
;;; GitHub libmsx project
;;; https://github.com/mori0091/libmsx

;;; \file crt0/init/init_0.s
;;; One of an `init` module for MSX ROM image.
;;;
;;; `init` of this starts immediately `main()` in initialization phase of the
;;; ROM cartridge. Application starts on Z80 mode with no system extension
;;; support.

        .module init_0

        .globl  start

        .area   _CODE
init::
        jp      start
