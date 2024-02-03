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
 * \file vgm2la.cpp
 */

#include "VGMFile.hpp"
#include "VGMParser.hpp"

#include <iostream>
#include <sstream>

int opt_rate = 60;
std::vector<std::string> infiles;
std::string outfile("out.la0");

[[noreturn]]
void help() {
  std::cout << "Usage: vgm2la [options] [FILE]..." << std::endl
            << std::endl
            << "Convert VGM files to the libmsx audio file (LA0)." << std::endl
            << "VGM files (*.vgm) or GZip compressed VGM files (*.vgz) can be used as FILE." << std::endl
            << "All FILEs will be converted and stored into one LA0 file." << std::endl
            << "(An LA0 file can contain multiple songs)." << std::endl
            << std::endl
            << "Options:" << std::endl
            << "  -h, --help     Display this help and exits." << std::endl
            << "  -o LA0_FILE    Place the output into LA0_FILE." << std::endl
            << "                 (if not specified, '-o out.la0' is assumed.)" << std::endl
            << "  -r, --rate HZ  Specify the default rate in Hz." << std::endl
            << "                 (if not specified, '-r 60' is assumed.)" << std::endl
            << std::endl
            << "Supported sound chip:" << std::endl
            << "  AY-3-8910 (PSG)" << std::endl
            << "  K051649 (SCC) / K052539 (SCC+)" << std::endl
            << "  YM2413 (OPLL)" << std::endl
            << "  (any other sound chips are not supported.)" << std::endl
            << std::endl;
  exit(0);
}

void parse_args(int argc, char ** argv) {
  if (argc < 2) {
    help();
  }
  for (int i = 1; i < argc; ++i) {
    if ('-' == argv[i][0]) {
      std::string opt{argv[i]};
      if (opt == "-h" || opt == "--help") {
        help();
      }
      else if (opt == "-o") {
        if (i+1 >= argc) {
          std::cerr << "Requires an argument for '" << opt << "'." << std::endl;
          exit(1);
        }
        outfile = std::string(argv[++i]);
      }
      else if (opt == "-r" || opt == "--rate") {
        if (i+1 >= argc) {
          std::cerr << "Requires an argument for '" << opt << "'." << std::endl;
          exit(1);
        }
        std::string arg{argv[++i]};
        std::istringstream(arg) >> opt_rate;
        if (opt_rate <= 0 || 240 < opt_rate) {
          std::cerr << "Illegal argument for '" << opt << "'." << std::endl;
          std::cerr << "Expects 1 to 240, but was \"" << arg << "\"." << std::endl;
          exit(1);
        }
      }
      else {
        std::cerr << "Unkown option - '" << opt << "'." << std::endl;
        exit(1);
      }
    }
    else {
      infiles.push_back(argv[i]);
    }
  }
}

int main(int argc, char ** argv) {
  parse_args(argc, argv);
  std::ofstream os = file_open_w(outfile);
  for (auto infile : infiles)
  {
    VGMFile vgm = VGMParser::parse(infile, opt_rate);
    std::cout << infile << std::endl;
    vgm.print();
    std::cout << std::endl;
    vgm.dump(os);
  }
  return 0;
}
