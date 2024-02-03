# -*- coding: utf-8-unix; tab-width: 8 -*-

NAME = vgm2la

LIBMSX_HOME = ../..

SRCDIR ?= src
OBJDIR ?= obj
BINDIR ?= bin

# CFLAGS = -std=c11 -Wall -Wpedantic
CFLAGS = -std=c11
CXXFLAGS = -std=c++17
LDFLAGS = -lz

# ----------------------------------------------
TARGETS = ${BINDIR}/${NAME}

CFLAGS ?=
CFLAGS += -MMD
CFLAGS += -I ${LIBMSX_HOME}/include

CXXFLAGS ?= --std=c++17
CXXFLAGS += -MMD
CXXFLAGS += -I ${LIBMSX_HOME}/include

# list of auto-generated source files
BUILT_SOURCES ?=

# # -- .aks files --
# AKS2C = ${LIBMSX_HOME}/tools/aks2c.sh

# SRCS_AKS ?= $(shell find ${SRCDIR} -type f -name '*.aks')
# SRCS_AKS_C = $(patsubst %.aks, %.c, ${SRCS_AKS})
# SRCS_AKS_H = $(patsubst %.aks, %.h, ${SRCS_AKS})

# BUILT_SOURCES += ${SRCS_AKS_C} ${SRCS_AKS_H}

SRCS ?= $(filter %.c, ${BUILT_SOURCES})
OBJS = $(patsubst ${SRCDIR}/%.c, ${OBJDIR}/%.o, ${SRCS})

SRCS_C = $(shell find ${SRCDIR} -type f -name '*.c')
OBJS += $(patsubst ${SRCDIR}/%.c, ${OBJDIR}/%.o, ${SRCS_C})

SRCS_CXX = $(shell find ${SRCDIR} -type f -name '*.cpp')
OBJS += $(patsubst ${SRCDIR}/%.cpp, ${OBJDIR}/%.o, ${SRCS_CXX})

DEPS = $(patsubst %.o, %.d, $(OBJS))

.PHONY: all build clean

all: build

build: ${TARGETS}

clean:
	@rm -f ${TARGETS} ${OBJS} ${DEPS} ${BUILT_SOURCES}
	@rm -rf ${OBJDIR} ${BINDIR}

# ${BINDIR}/${NAME}: ${OBJS}
# 	@$(info [BUILD] $@)
# 	@mkdir -p $(dir $@)
# 	@${CC} ${CFLAGS} -o $@ $^
${BINDIR}/${NAME}: ${OBJS}
	@$(info [BUILD] $@)
	@mkdir -p $(dir $@)
	@${CXX} ${CXXFLAGS} -o $@ $^ ${LDFLAGS}

${OBJDIR}/%.o: ${SRCDIR}/%.c
	@${info [C]	$<}
	@mkdir -p $(dir $@)
	@${CC} ${CFLAGS} -o $@ -c $<

${OBJDIR}/%.o: ${SRCDIR}/%.cpp
	@${info [C++]	$<}
	@mkdir -p $(dir $@)
	@${CXX} ${CXXFLAGS} -o $@ -c $<

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
