# -*- coding: utf-8-unix; tab-width: 8 -*-

# Copyright (c) 2021 Daishi Mori (mori0091)
#
# This software is released under the MIT License.
# See https://github.com/mori0091/libmsx/blob/main/LICENSE
#
# GitHub libmsx project
# https://github.com/mori0091/libmsx

# ------------------------------------------------------------------------
# configurations for making 16KB ROM image for MSX
# ------------------------------------------------------------------------

IMAGE_SIZE = 16384
ADDR_HEAD = 0x4000
ADDR_CODE = 0x4010
ADDR_DATA = 0xc000

IHX2BIN_FLAGS = -s ${IMAGE_SIZE} -b ${ADDR_HEAD}

ifeq (${USE_ALL_EXTENSIONS}, 1)
CRT0 = ${LIBMSX_HOME}/lib/16k.4000/crt0x.rel
else
CRT0 = ${LIBMSX_HOME}/lib/16k.4000/crt0.rel
endif
