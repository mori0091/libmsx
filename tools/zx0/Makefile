# -*- coding: utf-8-unix; tab-width: 8 -*-

NAME = zx0

SRCDIR ?= src
OBJDIR ?= obj
BINDIR ?= bin

TARGET = ${BINDIR}/${NAME}

CFLAGS ?= -std=c11 -Wall -Wpedantic
CFLAGS += -DNDEBUG -O2
CFLAGS += -MMD

SRCS = $(shell find ${SRCDIR} -type f -name '*.c')
OBJS = $(patsubst ${SRCDIR}/%.c, ${OBJDIR}/%.o, ${SRCS})

DEPS = $(patsubst %.o, %.d, $(OBJS))

.PHONY: all build clean

all: build

build: ${TARGET}

clean:
	@rm -f ${TARGETS} ${OBJS} ${DEPS}
	@rm -rf ${OBJDIR} ${BINDIR}

${BINDIR}/${NAME}: ${OBJS}
	@$(info [BUILD] $@)
	@mkdir -p $(dir $@)
	@${CC} ${CFLAGS} -o $@ $^

${OBJDIR}/%.o: ${SRCDIR}/%.c
	@${info [C]	$<}
	@mkdir -p $(dir $@)
	@${CC} ${CFLAGS} -o $@ -c $<
