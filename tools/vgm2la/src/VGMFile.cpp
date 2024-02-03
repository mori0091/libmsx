// -*- coding: utf-8-unix -*-
/*
 * Copyright (c) 2021-2023 Daishi Mori (mori0091)
 *
 * This software is released under the MIT License.\n
 * See https://github.com/mori0091/libmsx/blob/main/LICENSE
 *
 * GitHub libmsx project\n
 * https://github.com/mori0091/libmsx
 */
/**
 * \file VGMFile.cpp
 */

#include "VGMFile.hpp"
#include "GD3.hpp"
#include <cmath>
#include <cstddef>
#include <ostream>
#include <vector>

void VGMFile::print() {
  header.print();
  gd3_tags.print();
  if (samples.soundchip_enable & 2) {
    std::cerr << "SCC+ feature is used." << std::endl;
  }
}

void VGMFile::parse(std::istream & is, int opt_rate) {
  bool isEnd = false;
  SoundChip soundchip;

  header.parse(is);
  if (!header.K051649_K052539_clock &&
      !header.AY8910_clock &&
      !header.YM2413_clock)
  {
    // None of the supported sound chips have been used.
    return;
  }

  if (0 < header.rate) {
    samples.rate = header.rate;
  }
  else if (0 < opt_rate) {
    samples.rate = opt_rate;
  }
  else {
    samples.rate = 60;
  }

  const unsigned long vgm_freq = 44100;     // Sampling frequency of VGM in Hz.
  const unsigned long vsync = samples.rate; // Resampling frequency in Hz.

  auto counter = 0;
  while (!is.fail() && !isEnd) {
    auto cmd = u8(is);
    switch (cmd) {
      case 0x30: case 0x31: case 0x32: case 0x33:
      case 0x34: case 0x35: case 0x36: case 0x37:
      case 0x38: case 0x39: case 0x3A: case 0x3B:
      case 0x3C: case 0x3D: case 0x3E: case 0x3F:
        // one operand, reserved for future use
        // \note used for dual-chip support.
        u8(is);
        break;
      case 0x40: case 0x41: case 0x42: case 0x43:
      case 0x44: case 0x45: case 0x46: case 0x47:
      case 0x48: case 0x49: case 0x4A: case 0x4B:
      case 0x4C: case 0x4D: case 0x4E:
        // two operands, reserved for future use
        // \note was one operand only til v1.60
        if (0x160 < header.version) {
          u8(is); u8(is);
        }
        else {
          u8(is);
        }
        break;
      case 0x4F:                // Game Gear PSG stereo (not supported)
      case 0x50:                // PSG (SN76489/SN76496) (not supported)
      {
        u8(is);
        break;
      }
      case 0x51:                // YM2413 (OPLL)
      {
        auto reg = u8(is);
        auto val = u8(is);
        soundchip.OPLL_set(reg, val);
        break;
      }
      case 0x52:                // YM2612 port 0 (not supported)
      case 0x53:                // YM2612 port 1 (not supported)
      case 0x54:                // YM2151 (not supported)
      case 0x55:                // YM2203 (not supported)
      case 0x56:                // YM2608 port 0 (not supported)
      case 0x57:                // YM2608 port 1 (not supported)
      case 0x58:                // YM2610 port 0 (not supported)
      case 0x59:                // YM2610 port 1 (not supported)
      case 0x5A:                // YM3812 (not supported)
      case 0x5B:                // YM3526 (not supported)
      case 0x5C:                // YM8950 (not supported)
      case 0x5D:                // YMZ280B (not supported)
      case 0x5E:                // YMF262 port 0 (not supported)
      case 0x5F:                // YMF262 port 1 (not supported)
      {
        u8(is);
        u8(is);
        break;
      }
      case 0x61:
        counter += u16(is) * vsync;
        break;
      case 0x62:
        counter += 735 * vsync;
        break;
      case 0x63:
        counter += 882 * vsync;
        break;
      case 0x66:
        isEnd = true;
        break;
      // case 0x67:                // data block (not supported)
      //   break;
      // case 0x68:                // PCM RAM write (not supported)
      //   break;
      case 0x70: case 0x71: case 0x72: case 0x73:
      case 0x74: case 0x75: case 0x76: case 0x77:
      case 0x78: case 0x79: case 0x7A: case 0x7B:
      case 0x7C: case 0x7D: case 0x7E: case 0x7F:
        counter += ((cmd & 15) + 1) * vsync;
        break;
      // case 0x80: case 0x81: case 0x82: case 0x83:
      // case 0x84: case 0x85: case 0x86: case 0x87:
      // case 0x88: case 0x89: case 0x8A: case 0x8B:
      // case 0x8C: case 0x8D: case 0x8E: case 0x8F:
      // {
      //   // YM2612 port 0 address 2A write from the data bank, then wait n samples; n can range from 0 to 15.
      //   break;
      // }
      // case 0x90:
      // case 0x91:
      // case 0x92:
      // case 0x93:
      // case 0x94:
      // case 0x95:
      // {
      //   // DAC Stream Control Write
      //   break;
      // }
      case 0xA0:
      {
        // AY-3-8910
        auto reg = u8(is);
        auto val = u8(is);
        soundchip.PSG_set(reg, val);
        break;
      }
      case 0xA1: case 0xA2: case 0xA3:
      case 0xA4: case 0xA5: case 0xA6: case 0xA7:
      case 0xA8: case 0xA9: case 0xAA: case 0xAB:
      case 0xAC: case 0xAD: case 0xAE: case 0xAF:
        // two operands, reserved for future use
        // \note used for dual-chip support.
        u8(is); u8(is);
        break;
      case 0xB0:                // RF5C68 (not supported)
      case 0xB1:                // RF5C164 (not supported)
      case 0xB2:                // PWM (not supported)
      case 0xB3:                // GameBoy DMG (not supported)
      case 0xB4:                // NES APU (not supported)
      case 0xB5:                // MultiPCM (not supported)
      case 0xB6:                // uPD7759 (not supported)
      case 0xB7:                // OKIM6258 (not supported)
      case 0xB8:                // OKIM6295 (not supported)
      case 0xB9:                // HuC6280 (not supported)
      case 0xBA:                // K053260 (not supported)
      case 0xBB:                // Pokey (not supported)
      case 0xBC:                // WonderSwan (not supported)
      case 0xBD:                // SAA1099 (not supported)
      case 0xBE:                // ES5506 (not supported)
      case 0xBF:                // GA20 (not supported)
      {
        u8(is);
        u8(is);
        break;
      }
      case 0xC0:                // Sega PCM (not supported)
      case 0xC1:                // RF5C68 (not supported)
      case 0xC2:                // RF5C164 (not supported)
      {
        u16(is);
        u8(is);
        break;
      }
      case 0xC3:                // MultiPCM (not supported)
      {
        u8(is);
        u16(is);
        break;
      }
      case 0xC4:                // QSound (not supported)
      case 0xC5:                // SCSP (not supported)
      case 0xC6:                // WonderSwan (not supported)
      case 0xC7:                // VSU (not supported)
      case 0xC8:                // X1-010 (not supported)
      {
        u8(is);
        u8(is);
        u8(is);
        break;
      }
      case 0xC9: case 0xCA: case 0xCB:
      case 0xCC: case 0xCD: case 0xCE: case 0xCF:
        // three operands, reserved for future use
        u8(is); u8(is); u8(is);
        break;
      case 0xD0:                // YMF278B (not supported)
      case 0xD1:                // YMF271 (not supported)
      {
        u8(is);
        u8(is);
        u8(is);
        break;
      }
      case 0xD2:
      {
        // K051649 (SCC) / K052539 (SCC+)
        auto port = u8(is);
        auto reg = u8(is);
        auto val = u8(is);
        soundchip.SCC_set(port, reg, val);
        break;
      }
      case 0xD3:                // K054539 (not supported)
      case 0xD4:                // C140 (not supported)
      case 0xD5:                // ES5503 (not supported)
      case 0xD6:                // ES5506 (not supported)
      {
        u8(is);
        u8(is);
        u8(is);
        break;
      }
      case 0xD7:
      case 0xD8: case 0xD9: case 0xDA: case 0xDB:
      case 0xDC: case 0xDD: case 0xDE: case 0xDF:
        // three operands, reserved for future use
        u8(is); u8(is); u8(is);
        break;
      case 0xE0:
      {
        // Seek to offset dddddddd (Intel byte order) in PCM data bank of data block type 0 (YM2612).
        // (not supported)
        u32(is);
        break;
      }
      case 0xE1:                // C352 (not supported)
      {
        u16(is);
        u16(is);
        break;
      }
      case 0xE2: case 0xE3:
      case 0xE4: case 0xE5: case 0xE6: case 0xE7:
      case 0xE8: case 0xE9: case 0xEA: case 0xEB:
      case 0xEC: case 0xED: case 0xEE: case 0xEF:
      case 0xF0: case 0xF1: case 0xF2: case 0xF3:
      case 0xF4: case 0xF5: case 0xF6: case 0xF7:
      case 0xF8: case 0xF9: case 0xFA: case 0xFB:
      case 0xFC: case 0xFD: case 0xFE: case 0xFF:
        // four operands, reserved for future use
        u8(is); u8(is); u8(is); u8(is);
        break;
      default:
        std::cerr << "Unknown VGM command - 0x" << std::hex << (int)cmd << std::endl;
        exit(1);
        break;
    }
    while (vgm_freq <= counter) {
      counter -= vgm_freq;
      samples.sample(soundchip);
    }
  }
  if (counter) {
    samples.sample(soundchip);
  }

  if (header.GD3_offset) {
    // \note cannot use seekg() or gcount() if `is` wasn't regular file, such as pipe or fifo.
    is.seekg(header.GD3_offset);
    gd3_tags.parse(is);
  }
}

void VGMFile::dump(std::ostream & os) {
  samples.dump(header, os);
}
