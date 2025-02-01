# -*- coding: utf-8-unix; tab-width: 8 -*-

# Copyright (c) 2021-2025 Daishi Mori (mori0091)
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
	${BINDIR}/zx0 \
	${BINDIR}/vgm2la

all: build

build: ${TARGETS}

clean:
	@rm -f ${TARGETS}
	@${MAKE} -s -C ihx2bin clean
	@${MAKE} -s -C zx0 clean
	@${MAKE} -s -C vgm2la clean

${BINDIR}/ihx2bin:
	@${MAKE} -s -C ihx2bin

${BINDIR}/zx0:
	@${MAKE} -s -C zx0

${BINDIR}/vgm2la:
	@${MAKE} -s -C vgm2la

${BINDIR}/%.sh: %.sh
	@cp $< $@

${BINDIR}/%.xsl: %.xsl
	@cp $< $@
