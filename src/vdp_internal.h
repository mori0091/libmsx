// -*- coding: utf-8-unix -*-

#ifndef VDP_INTERNAL_H
#define VDP_INTERNAL_H

#pragma once

#define VDP_SET_CTRL(reg, x)                    \
  vdp_port1 = (x);                              \
  vdp_port1 = ((reg) | 0x80)

#define VDP_SET_CTRL_PTR_NOINC(reg)             \
  VDP_SET_CTRL(17, (reg) | 0x80)

#define VDP_SET_CTRL_PTR(reg)                   \
  VDP_SET_CTRL(17, (reg))

#define VDP_SET_CTRL_VALUE(x)                   \
  vdp_port3 = (x)


extern volatile enum vdp_screen_mode screen_mode;

#define VDP_CMD_LMMV (0x80)
#define VDP_CMD_LMMM (0x90)
#define VDP_CMD_HMMV (0xc0)
#define VDP_CMD_HMMM (0xd0)

#endif
