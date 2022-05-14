# -*- coding: utf-8-unix; tab-width: 8 -*-

# Copyright (c) 2022 Daishi Mori (mori0091)
#
# This software is released under the MIT License.
# See https://github.com/mori0091/libmsx/blob/main/LICENSE
#
# GitHub libmsx project
# https://github.com/mori0091/libmsx

# ------------------------------------------------------------------------
# configurations for making Mega ROM image for MSX (ROM type: ASCII8)
# ------------------------------------------------------------------------

IMAGE_SIZE = 32768
ADDR_HEAD = 0x4000
ADDR_CODE = 0x4010
ADDR_DATA = 0xc000

CRT0 = ${LIBMSX_HOME}/lib/rom_mapper/crt0.rel
CRT0 += ${LIBMSX_HOME}/lib/rom_mapper/rom_ascii8.rel
