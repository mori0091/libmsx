// -*- coding: utf-8-unix -*-

#ifndef IO_H
#define IO_H

#pragma once

// ---- VDP I/O ports
static volatile __sfr __at 0x98 vdp_port0;
static volatile __sfr __at 0x99 vdp_port1;
static volatile __sfr __at 0x9a vdp_port2;
static volatile __sfr __at 0x9b vdp_port3;

// ---- PSG I/O ports
static volatile __sfr __at 0x0a0 psg_port0; /* address latch */
static volatile __sfr __at 0x0a1 psg_port1; /* data write */
static volatile __sfr __at 0x0a2 psg_port2; /* data read */

#endif
