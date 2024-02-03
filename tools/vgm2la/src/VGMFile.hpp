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
 * \file VGMFile.hpp
 */

#ifndef VGMFILE_HPP_
#define VGMFILE_HPP_

#include <istream>
#include <ostream>

#include "VGMHeader.hpp"
#include "AudioSample.hpp"
#include "GD3.hpp"

struct VGMFile {
  VGMFile() : header(), samples(), gd3_tags() {}

  void print();
  void parse(std::istream & is, int opt_rate = 60);
  void dump(std::ostream & os);

  VGMHeader header;
  AudioSample samples;
  GD3 gd3_tags;
};

#endif // VGMFILE_HPP_
