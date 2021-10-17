# -*- coding: utf-8-unix; tab-width: 8 -*-

# NAME =
# CFLAGS  = -DNDEBUG
# LDFLAGS =
# LDLIBS  =
#
# IMAGE_SIZE =
# ADDR_HEAD =
# ADDR_CODE =
# ADDR_DATA =
# CRT0 =

.PHONY: all build clean

all: build

SRCDIR ?= src
OBJDIR ?= obj
BINDIR ?= bin

SRCS_C = $(shell find ${SRCDIR} -type f -name '*.c')
OBJS_C = $(patsubst ${SRCDIR}/%.c, ${OBJDIR}/%.rel, ${SRCS_C})

SRCS_ASM = $(shell find ${SRCDIR} -type f -name '*.s')
OBJS_ASM = $(patsubst ${SRCDIR}/%.s, ${OBJDIR}/%.rel, ${SRCS_ASM})

OBJS = ${OBJS_ASM} ${OBJS_C}

DEPS = $(patsubst %.rel, %.d, $(OBJS))

LIBMSX = libmsx/lib/libmsx.lib
LIBS = ${LIBMSX}

TARGETS = \
	${BINDIR}/${NAME}.dat \
	${BINDIR}/${NAME}.rom

CC = sdcc
AS = sdasz80
LD = sdld
AR = sdar
OBJCOPY = sdobjcopy

CFLAGS ?=
CFLAGS += -mz80 -MMD
CFLAGS += -I libmsx/include

LDFLAGS ?=
LDFLAGS += --no-std-crt0 --code-loc ${ADDR_CODE} --data-loc ${ADDR_DATA}

LDLIBS ?=

build: lib ${TARGETS}

clean:
	@rm -f ${TARGETS} ${OBJS} ${DEPS}
	@rm -rf ${OBJDIR} ${BINDIR}
	@${MAKE} -s -C libmsx clean

lib ${LIBMSX}:
	@${MAKE} -C libmsx

%.rom: %.ihx
	@${info [Build]	$@}
	@${OBJCOPY} -I ihex -O binary \
	--gap-fill 0xff \
	--pad-to $$(printf '%d' $$((${ADDR_HEAD} + ${IMAGE_SIZE}))) \
	$< $@

%.dat: %.ihx
	@${info [Build]	$@}
	@${OBJCOPY} -I ihex -O binary $< $@

${BINDIR}/${NAME}.ihx: ${CRT0} ${OBJS} ${LIBS}
	@${info [LD]	$@}
	@mkdir -p $(dir $@)
	@${CC} ${CFLAGS} -o $@ ${LDFLAGS} $^ ${LDLIBS}

${OBJDIR}/%.rel: ${SRCDIR}/%.c
	@${info [C]	$<}
	@mkdir -p $(dir $@)
	@${CC} ${CFLAGS} -o $@ -c $<

${OBJDIR}/%.rel: ${SRCDIR}/%.s
	@${info [AS]	$<}
	@mkdir -p $(dir $@)
	@${AS} -o $@ $<

crt0/%.rel: crt0/%.s
	@${info [AS]	$<}
	@mkdir -p $(dir $@)
	@${AS} -g -o $@ $<

-include $(DEPS)
