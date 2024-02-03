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
 * \file VGMParser.cpp
 */

#include "VGMParser.hpp"

VGMFile VGMParser::parse(const std::string & filename, int opt_rate) {
  auto v = load_gzfile(filename);
  auto sb = streambuf_from_vector(v);
  std::istream is{&sb};
  VGMFile vgm;
  vgm.parse(is, opt_rate);
  return vgm;
}
