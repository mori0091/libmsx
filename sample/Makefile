# -*- coding: utf-8-unix; tab-width: 8 -*-

# Copyright (c) 2021-2023 Daishi Mori (mori0091)
#
# This software is released under the MIT License.
# See https://github.com/mori0091/libmsx/blob/main/LICENSE
#
# GitHub libmsx project
# https://github.com/mori0091/libmsx

.PHONY: all build clean

SAMPLE_PROJECTS = \
	$(dir $(shell find . -mindepth 2 -maxdepth 2 -type f -name 'Makefile'))

all: build

build:
	@$(foreach d, ${SAMPLE_PROJECTS}, echo '${d}'; ${MAKE} -s -C ${d} ; )

clean:
	@$(foreach d, ${SAMPLE_PROJECTS}, ${MAKE} -s -C ${d} clean ; )
