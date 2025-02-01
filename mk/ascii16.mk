# -*- coding: utf-8-unix; tab-width: 8 -*-

# Copyright (c) 2021-2025 Daishi Mori (mori0091)
#
# This software is released under the MIT License.\n
# See https://github.com/mori0091/libmsx/blob/main/LICENSE
#
# GitHub libmsx project\n
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
#         |            | --> | CODE       |                           /  |  (BANK1)   |
#         |            |     | etc.       |                          /   |            |
# 0x08000 |------------|.....|------------+------------+------------+    |------------| 0x08000
#         | page #2    |     | BANK1      | BANK2      | BANK3      |    | segment #2 |
#         |            | --> |            |            |            |    |  (BANK2)   |
#         |            |     |            |            |            |    |            |
# 0x0C000 |------------|.....+------------+------------+------------+    |------------| 0x0C000
#         | page #3    |                                             .   | segment #3 |
#         | RAM        | page #1 is mapped to segment #0 (always)     .  |  (BANK3)   |
#         |            | page #2 is mapped to segment #1 (initially)   . |            |
# 0x10000 +------------+ page #2 is used as 16 KiB banked ROM area      .|------------| 0x10000
#

CONFIG_ROM_TYPE = ascii16
CONFIG_CRT0_MOD_INIT ?= init_0
