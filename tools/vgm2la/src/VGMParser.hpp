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
 * \file VGMParser.hpp
 */

#ifndef VGMPARSER_HPP_
#define VGMPARSER_HPP_

#include "VGMFile.hpp"

namespace VGMParser {
  VGMFile parse(const std::string & filename, double opt_rate = 0.0);
}

#endif // VGMPARSER_HPP_
