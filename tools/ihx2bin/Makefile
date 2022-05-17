# -*- coding: utf-8-unix; tab-width: 8 -*-

TARGET = bin/ihx2bin

CFLAGS = -std=c11 -Wall -Wpedantic

SRCS = src/ihx2bin.c

.PHONY: all build clean

all: build

build: ${TARGET}

clean:
	@rm -rf bin

${TARGET}: ${SRCS}
	@$(info [BUILD] $@)
	@mkdir -p $(dir $@)
	@${CC} ${CFLAGS} -o $@ $^
