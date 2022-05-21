# -*- coding: utf-8-unix; tab-width: 8 -*-

SRCDIR ?= src
OBJDIR ?= obj
BINDIR ?= bin

TARGET = ${BINDIR}/ihx2bin

CFLAGS = -std=c11 -Wall -Wpedantic

SRCS = ${SRCDIR}/ihx2bin.c

.PHONY: all build clean

all: build

build: ${TARGET}

clean:
	@rm -f ${TARGET}

${TARGET}: ${SRCS}
	@$(info [BUILD] $@)
	@mkdir -p $(dir $@)
	@${CC} ${CFLAGS} -o $@ $^
