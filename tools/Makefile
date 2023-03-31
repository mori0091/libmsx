# -*- coding: utf-8-unix; tab-width: 8 -*-

# Copyright (c) 2021-2023 Daishi Mori (mori0091)
#
# This software is released under the MIT License.
# See https://github.com/mori0091/libmsx/blob/main/LICENSE
#
# GitHub libmsx project
# https://github.com/mori0091/libmsx

.PHONY: all build clean

BINDIR ?= ../bin

TARGETS = \
	${BINDIR}/ihx2bin \
	${BINDIR}/aks2c.sh \
	${BINDIR}/aks2c.xsl \
	${BINDIR}/pad2pow2.sh \
	${BINDIR}/ridxc.sh \
	${BINDIR}/zx0

all: build

build: ${TARGETS}

clean:
	@rm -f ${TARGETS}
	@${MAKE} -s -C ihx2bin clean
	@${MAKE} -s -C zx0 clean

${BINDIR}/ihx2bin:
	@${MAKE} -s -C ihx2bin

${BINDIR}/zx0:
	@${MAKE} -s -C zx0

${BINDIR}/%.sh: %.sh
	@cp $< $@

${BINDIR}/%.xsl: %.xsl
	@cp $< $@
