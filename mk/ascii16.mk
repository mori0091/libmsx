# -*- coding: utf-8-unix; tab-width: 8 -*-

# Copyright (c) 2022 Daishi Mori (mori0091)
#
# This software is released under the MIT License.
# See https://github.com/mori0091/libmsx/blob/main/LICENSE
#
# GitHub libmsx project
# https://github.com/mori0091/libmsx

# ------------------------------------------------------------------------
# configurations for making Mega ROM image for MSX (ROM type: ASCII16)
# ------------------------------------------------------------------------

#         Z80 / R800                                                     ROM cartridge
#         address space                                                  address space
# 0x00000 +------------+                                                .+------------+ 0x00000
#         | page #0    |                                               / | segment #0 |
#         | MAIN ROM   |     SDCC                                     /  |            |
#         |            |     segment names                           /   |            |
# 0x04000 |------------|.....+------------+..........................   .|------------| 0x04000
#         | page #1    |     | HEADER     |                            / | segment #1 |
#         |            | --> | CODE       |                           /  |  (BANK0)   |
#         |            |     | etc.       |                          /   |            |
# 0x08000 |------------|.....|------------+------------+------------+    |------------| 0x08000
#         | page #2    |     | BANK0      | BANK1      | BANK2      |    | segment #2 |
#         |            | --> |            |            |            |    |  (BANK1)   |
#         |            |     |            |            |            |    |            |
# 0x0C000 |------------|.....+------------+------------+------------+    |------------| 0x0C000
#         | page #3    |                                             .   | segment #3 |
#         | RAM        | page #1 is mapped to segment #0 (always)     .  |  (BANK2)   |
#         |            | page #2 is mapped to segment #1 (initially)   . |            |
# 0x10000 +------------+ page #2 is used as 16 KiB banked ROM area      .|------------| 0x10000
#

IMAGE_SIZE = 16384
ADDR_HEAD = 0x4000
ADDR_CODE = 0x4010
ADDR_DATA = 0xc000

LDFLAGS += \
	-Wl-b_BANK0=0x08000 \
	-Wl-b_BANK1=0x18000 \
	-Wl-b_BANK2=0x28000 \
	-Wl-b_BANK3=0x38000 \
	-Wl-b_BANK4=0x48000 \
	-Wl-b_BANK5=0x58000 \
	-Wl-b_BANK6=0x68000

IHX2BIN_FLAGS = -s ${IMAGE_SIZE} -b ${ADDR_HEAD} \
		-s 16384			 \
		-b 0x08000			 \
		-b 0x18000			 \
		-b 0x28000			 \
		-b 0x38000			 \
		-b 0x48000			 \
		-b 0x58000			 \
		-b 0x68000			 \
		--pow2

CRT0 = ${LIBMSX_HOME}/lib/rom_mapper/crt0.rel
CRT0 += ${LIBMSX_HOME}/lib/rom_mapper/rom_ascii16.rel
