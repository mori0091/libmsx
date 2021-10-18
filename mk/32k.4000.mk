# -*- coding: utf-8-unix; tab-width: 8 -*-

# ------------------------------------------------------------------------
# configurations for making 32KB ROM image for MSX
# ------------------------------------------------------------------------

IMAGE_SIZE = 32768
ADDR_HEAD = 0x4000
ADDR_CODE = 0x4020
ADDR_DATA = 0xc000

CRT0 = ${LIBMSX_HOME}/lib/32k.4000/crt0.rel
