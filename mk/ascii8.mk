# -*- coding: utf-8-unix; tab-width: 8 -*-

# Copyright (c) 2021-2025 Daishi Mori (mori0091)
#
# This software is released under the MIT License.\n
# See https://github.com/mori0091/libmsx/blob/main/LICENSE
#
# GitHub libmsx project\n
# https://github.com/mori0091/libmsx

# ------------------------------------------------------------------------
# configurations for making Mega ROM image for MSX (ROM type: ASCII8)
# ------------------------------------------------------------------------

#         Z80 / R800                                                     ROM cartridge
#         address space                                                  address space
# 0x00000 +------------+                                                .+------------+ 0x00000
#         | page #0    |                                               / | segment #0 |
#         | MAIN ROM   |     SDCC                                     /  |------------| 0x20000
#         |            |     segment names                           /   | segment #1 |
# 0x04000 |------------|.....+------------+..........................   .|------------| 0x04000
#         | page #1    |     | HEADER     |                            / | segment #2 |
#         |            | --> | CODE       |                           /  |- (BANK1) --| 0x06000
#         |            |     | etc.       |                          /   | segment #3 |
# 0x08000 |------------|.....|------------+------------+------------+    |------------| 0x08000
#         | page #2    |     | BANK1      | BANK2      | BANK3      |    | segment #4 |
#         |            | --> |            |            |            |    |- (BANK2) --| 0x0A000
#         |            |     |            |            |            |    | segment #5 |
# 0x0C000 |------------|.....+------------+------------+------------+    |------------| 0x0C000
#         | page #3    |                                             .   | segment #6 |
#         | RAM        | page #1 is mapped to segment #0-1 (always)   .  |- (BANK3) --| 0x0E000
#         |            | page #2 is mapped to segment #2-3 (initially) . | segment #7 |
# 0x10000 +------------+ page #2 is used as 16 KiB banked ROM area      .|------------| 0x10000
#

CONFIG_ROM_TYPE = ascii8
CONFIG_CRT0_MOD_INIT ?= init_0
