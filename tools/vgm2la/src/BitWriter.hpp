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
 * \file BitWriter.hpp
 */

#ifndef BITWRITER_HPP_
#define BITWRITER_HPP_

#include <cstdint>
#include <cstddef>
#include <vector>

struct BitWriter {
  uint8_t bitmask;
  size_t pbits_idx;
  std::vector<uint8_t> out;

  BitWriter() : bitmask(0), pbits_idx(0), out() {}

  void write_byte(uint8_t b) {
    out.push_back(b);
  }

  void write_bit(bool b) {
    if (!bitmask) {
      bitmask = 0x80;
      pbits_idx = out.size();
      out.push_back(0);
    }
    if (b) {
      out[pbits_idx] |= bitmask;
    }
    bitmask >>= 1;
  }

};

#endif // BITWRITER_HPP_
