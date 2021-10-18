# -*- coding: utf-8-unix; tab-width: 8 -*-

.PHONY: all build clean

SAMPLE_PROJECTS = \
	$(dir $(shell find . -mindepth 2 -maxdepth 2 -type f -name 'Makefile'))

all: build

build:
	@$(foreach d, ${SAMPLE_PROJECTS}, echo '${d}'; ${MAKE} -s -C ${d} ; )

clean:
	@$(foreach d, ${SAMPLE_PROJECTS}, ${MAKE} -s -C ${d} clean ; )
