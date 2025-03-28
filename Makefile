# -*- coding: utf-8-unix; tab-width: 8 -*-

# Copyright (c) 2021-2025 Daishi Mori (mori0091)
#
# This software is released under the MIT License.
# See https://github.com/mori0091/libmsx/blob/main/LICENSE
#
# GitHub libmsx project
# https://github.com/mori0091/libmsx

.PHONY: all build clean sample tools test

all: build tools

SRCDIR ?= src
OBJDIR ?= obj
BINDIR ?= bin
LIBDIR ?= lib

NAME = libmsx

SRCS_C = $(shell find ${SRCDIR} -type f -name '*.c')
OBJS_C = $(patsubst ${SRCDIR}/%.c, ${OBJDIR}/%.rel, ${SRCS_C})

SRCS_ASM = $(shell find ${SRCDIR} -type f -name '*.s')
OBJS_ASM = $(patsubst ${SRCDIR}/%.s, ${OBJDIR}/%.rel, ${SRCS_ASM})

OBJS = ${OBJS_ASM} ${OBJS_C}

SRCS_CRT0 = $(shell find crt0 -type f -name '*.s')
OBJS_CRT0 = $(patsubst crt0/%.s, ${LIBDIR}/%.rel, ${SRCS_CRT0})

DEPS = $(patsubst %.rel, %.d, $(OBJS))

TARGETS = \
	${OBJS_CRT0} \
	${LIBDIR}/${NAME}.lib

SDCC = sdcc
SDAS = sdasz80
SDLD = sdld
SDAR = sdar

CFLAGS ?= --opt-code-size
CFLAGS += -mz80 -MMD
CFLAGS += -DNDEBUG --Werror
CFLAGS += -I include
LDFLAGS = --no-std-crt0 --code-loc ${ADDR_CODE} --data-loc ${ADDR_DATA}

build: ${TARGETS}

clean:
	@rm -f ${TARGETS} ${OBJS} ${OBJS_CRT0} ${DEPS}
	@rm -rf ${OBJDIR} ${BINDIR} ${LIBDIR}
	@${MAKE} -s -C sample clean
	@make BINDIR="$(abspath ${BINDIR})" -s -C tools clean
	@make -s -C test clean

sample:
	@${MAKE} -s -C sample

test:
	@make -s -C test

tools:
	@make BINDIR="$(abspath ${BINDIR})" -s -C tools

${LIBDIR}/%.rel: crt0/%.s
	@${info [AS]	$<}
	@mkdir -p $(dir $@)
	@${SDAS} -g -o $@ $<

${LIBDIR}/${NAME}.lib: ${OBJS}
	@${info [AR]	$@}
	@mkdir -p $(dir $@)
	@$(SDAR) cr $@ $^

${OBJDIR}/%.rel: ${SRCDIR}/%.c
	@${info [C]	$<}
	@mkdir -p $(dir $@)
	@${SDCC} ${CFLAGS} -o $@ -c $<

${OBJDIR}/%.rel: ${SRCDIR}/%.s
	@${info [AS]	$<}
	@mkdir -p $(dir $@)
	@${SDAS} -o $@ $<

-include $(DEPS)
