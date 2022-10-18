# -*- coding: utf-8-unix; tab-width: 8 -*-

# Copyright (c) 2021 Daishi Mori (mori0091)
#
# This software is released under the MIT License.
# See https://github.com/mori0091/libmsx/blob/main/LICENSE
#
# GitHub libmsx project
# https://github.com/mori0091/libmsx

# LIBMSX_HOME = ./libmsx
#
# NAME = foo
#
# CFLAGS  = -DNDEBUG
# LDFLAGS =
# LDLIBS  =
#
# IMAGE_SIZE = 32768
# ADDR_HEAD = 0x4000
# ADDR_CODE = 0x4020
# ADDR_DATA = 0xc000
# CRT0 = ${LIBMSX_HOME}/lib/32k.4000/crt0.rel

.PHONY: all build clean libmsx

all: build

SRCDIR ?= src
OBJDIR ?= obj
BINDIR ?= bin

# list of auto-generated source files
BUILT_SOURCES ?=

AKS2C = ${LIBMSX_HOME}/tools/aks2c.sh

SRCS_AKS ?= $(shell find ${SRCDIR} -type f -name '*.aks')
SRCS_AKS_C = $(patsubst %.aks, %.c, ${SRCS_AKS})
SRCS_AKS_H = $(patsubst %.aks, %.h, ${SRCS_AKS})

BUILT_SOURCES += ${SRCS_AKS_C} ${SRCS_AKS_H}

SRCS_C ?= $(filter %.c, ${BUILT_SOURCES})
SRCS_C += $(shell find ${SRCDIR} -type f -name '*.c')
OBJS_C = $(patsubst ${SRCDIR}/%.c, ${OBJDIR}/%.rel, ${SRCS_C})

SRCS_ASM = $(shell find ${SRCDIR} -type f -name '*.s')
OBJS_ASM = $(patsubst ${SRCDIR}/%.s, ${OBJDIR}/%.rel, ${SRCS_ASM})

OBJS = ${OBJS_ASM} ${OBJS_C}

DEPS = $(patsubst %.rel, %.d, $(OBJS))

TARGETS = \
	${BINDIR}/${NAME}.dat \
	${BINDIR}/${NAME}.rom

CC = sdcc
AS = sdasz80
LD = sdld
AR = sdar
OBJCOPY = sdobjcopy
IHX2BIN = ${LIBMSX_HOME}/bin/ihx2bin

CFLAGS ?=
CFLAGS += -mz80 -MMD
CFLAGS += -I ${LIBMSX_HOME}/include

LIBMSX = ${LIBMSX_HOME}/lib/libmsx.lib

LIBS ?=
LIBS += ${LIBMSX}

LDFLAGS ?=
LDFLAGS += --no-std-crt0 --code-loc ${ADDR_CODE} --data-loc ${ADDR_DATA}
LDFLAGS += -L ${LIBMSX_HOME}/lib

LDLIBS ?=
# LDLIBS += -llibmsx

build: ${TARGETS}

clean:
	@rm -f ${TARGETS} ${OBJS} ${DEPS} ${BUILT_SOURCES}
	@rm -rf ${OBJDIR} ${BINDIR}

ifdef LIBMSX_HOME
libmsx:
	@${MAKE} -C ${LIBMSX_HOME}
else
libmsx:
	@${info LIBMSX_HOME is not defined in Makefile.}
	@${info LIBMSX_HOME shall be defined as follows:}
	@${info LIBMSX_HOME = <path to top of libmsx>}
endif

${CRT0} ${LIBMSX}: libmsx

%.rom: %.ihx
	@${info [Build]	$@}
	@${IHX2BIN} ${IHX2BIN_FLAGS} -o $@ $<

%.dat: %.ihx
	@${info [Build]	$@}
	@${IHX2BIN} -o $@ $<

# %.rom: %.ihx
# 	@${info [Build]	$@}
# 	@${OBJCOPY} -I ihex -O binary \
# 	--gap-fill 0xff \
# 	--pad-to $$(printf '%d' $$((${ADDR_HEAD} + ${IMAGE_SIZE}))) \
# 	$< $@

# %.dat: %.ihx
# 	@${info [Build]	$@}
# 	@${OBJCOPY} -I ihex -O binary $< $@

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

${SRCS_AKS_C}: %.c: %.aks | %.h
	@${info [AKS2C]	$< -> $@}
	@${AKS2C} --prefix "$(notdir $(basename $<))_" -o $@ $<

${SRCS_AKS_H}: %.h: %.aks
	@${info [AKS2C]	$< -> $@}
	@${AKS2C} --prefix "$(notdir $(basename $<))_" -o $@ $< --header

-include $(DEPS)
