# -*- coding: utf-8-unix; tab-width: 8 -*-

NAME = test_zx0

LIBMSX_HOME = ../..

SRCDIR ?= src
OBJDIR ?= obj
BINDIR ?= bin

# CFLAGS = -std=c11 -Wall -Wpedantic
CFLAGS = -std=c11

# ----------------------------------------------
TARGET = ${BINDIR}/${NAME}

CFLAGS ?=
CFLAGS += -MMD
CFLAGS += -DNDEBUG
CFLAGS += -I ${LIBMSX_HOME}/include

# list of auto-generated source files
BUILT_SOURCES ?=

# # -- .aks files --
# AKS2C = ${LIBMSX_HOME}/tools/aks2c.sh

# SRCS_AKS ?= $(shell find ${SRCDIR} -type f -name '*.aks')
# SRCS_AKS_C = $(patsubst %.aks, %.c, ${SRCS_AKS})
# SRCS_AKS_H = $(patsubst %.aks, %.h, ${SRCS_AKS})

# BUILT_SOURCES += ${SRCS_AKS_C} ${SRCS_AKS_H}

SRCS ?= $(filter %.c, ${BUILT_SOURCES})
SRCS += $(shell find ${SRCDIR} -type f -name '*.c')
OBJS = $(patsubst ${SRCDIR}/%.c, ${OBJDIR}/%.o, ${SRCS})

SRCS_ZX0_C = $(shell find ${LIBMSX_HOME}/src \
		-type f \
		-name 'mem_rw.c' -o \
		-name 'ZX0__decode.c' -o \
		-name 'ZX0_decompress.c' \
		)
OBJS += $(patsubst ${LIBMSX_HOME}/src/%.c, ${OBJDIR}/%.o, ${SRCS_ZX0_C})

DEPS = $(patsubst %.o, %.d, $(OBJS))

.PHONY: all build clean test

all: test

build: ${TARGET}

clean:
	@rm -f ${TARGET} ${OBJS} ${DEPS} ${BUILT_SOURCES}
	@rm -rf ${OBJDIR} ${BINDIR}

test: ${TARGET}
	${TARGET} hello.txt hello.txt.zx0
	${TARGET} KOTATSU2.S12 KOTATSU2.S12.zx0

${BINDIR}/${NAME}: ${OBJS}
	@$(info [BUILD] $@)
	@mkdir -p $(dir $@)
	@${CC} ${CFLAGS} -o $@ $^

${OBJDIR}/%.o: ${SRCDIR}/%.c
	@${info [C]	$<}
	@mkdir -p $(dir $@)
	@${CC} ${CFLAGS} -o $@ -c $<

${OBJDIR}/%.o: ${LIBMSX_HOME}/src/%.c
	@${info [C]	$<}
	@mkdir -p $(dir $@)
	@${CC} ${CFLAGS} -o $@ -c $<

# ${SRCS_AKS_C}: %.c: %.aks | %.h
# 	@${info [AKS2C]	$< -> $@}
# 	@${AKS2C} --name "$(notdir $(basename $<))" -o $@ $<

# ${SRCS_AKS_H}: %.h: %.aks
# 	@${info [AKS2C]	$< -> $@}
# 	@${AKS2C} --name "$(notdir $(basename $<))" -o $@ $< --header

-include $(DEPS)
