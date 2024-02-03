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
 * \file AudioSample.cpp
 */

#include "AudioSample.hpp"
#include "BitWriter.hpp"
#include "io.hpp"

#include <math.h>

static const uint8_t TAG_LA0_FILE[4] = {'L', 'A', '0', ' '};

static const uint8_t TAG_SCC_WAVETBL[4] = {'S', 'C', 'C', 'W'};
static const uint8_t TAG_STREAM_DATA[4] = {'S', 'T', 'R', 'M'};

static void write_tag(std::ostream & os, const uint8_t tag[4]) {
  os.write((const char *)tag, 4);
}

/**
 * Encode an integer value to an elias-gamma code and write to output.
 *
 * \param out a BitWriter.
 * \param x   a positive integer.
 */
static void write_elias_gamma(BitWriter & out, size_t x) {
  assert(x);
  size_t mask = 2;
  while (mask <= x) {
    out.write_bit(false);
    mask <<= 1;
  }
  while (mask >>= 1) {
    out.write_bit(!!(x & mask));
  }
}

/**
 * Encode an integer value to an interlaced elias-gamma code and write to
 * output.
 *
 * \param out a BitWriter.
 * \param x   a positive integer.
 */
static void write_interlaced_elias_gamma(BitWriter & out, size_t x) {
  assert(x);
  size_t mask = 2;
  while (mask <= x) {
    mask <<= 1;
  }
  mask >>= 1;
  while (mask >>= 1) {
    out.write_bit(false);
    out.write_bit(!!(x & mask));
  }
  out.write_bit(true);
}

static void encode_size(BitWriter & bw, size_t x) {
  // write_interlaced_elias_gamma(bw, x+1);
  write_elias_gamma(bw, x+1);
}

static void encode_offset(BitWriter & bw, int x) {
  size_t y = (x < 0) ? (2 * -x) : (2 * x + 1);
  // write_interlaced_elias_gamma(bw, y);
  write_elias_gamma(bw, y);
}

static void append(std::vector<uint8_t> & v, uint32_t x) {
  v.push_back((x >> 0) & 255);
  v.push_back((x >> 8) & 255);
  v.push_back((x >> 16) & 255);
  v.push_back((x >> 24) & 255);
}
static void append(std::vector<uint8_t> & v, const std::vector<uint8_t> & v2) {
  v.insert(v.end(), v2.begin(), v2.end());
}
static void append(std::vector<uint8_t> & v, const std::vector<std::vector<uint8_t>> & v2) {
  for (auto i : v2) {
    append(v, i);
  }
}

template <typename T>
std::pair<int, size_t> find_vec(T & x, std::vector<T> & db) {
  size_t offset = 0;
  for (int i = 0; i < db.size(); ++i) {
    if (x == db[i]) return std::make_pair(i, offset);
    offset += db[i].size();
  }
  int idx = db.size();
  db.push_back(x);
  return std::make_pair(idx, offset);
}

static std::vector<uint8_t> compress(const std::vector<SoundChip::Sample> & samples) {
  if (samples.empty()) return std::vector<uint8_t>();
  std::vector<std::vector<uint8_t>> db;
  BitWriter bw;
  size_t curr_pos = 0;
  for (auto x : samples) {
    if (x.empty()) {
      encode_size(bw, 0);       // Number of commands in the next sample.
      continue;
    }
    auto [idx, next_pos] = find_vec(x, db);
    size_t len = x.size()/2;
    int offset = (next_pos - curr_pos)/2;
    encode_size(bw, len);       // Number of commands in the next sample.
    encode_offset(bw, offset);  // Offset to the next sample (in 16-bit words).
    curr_pos = next_pos + x.size();
  }
  std::vector<uint8_t> ret;
  append(ret, bw.out.size());   // Size of SEQUENCE in bytes.
  append(ret, bw.out);          // SEQUENCE.
  append(ret, db);              // DICTIONARY.
  return ret;
}

void AudioSample::dump(const VGMHeader & vgm_header, std::ostream & os) {
  if (samples.empty()) {
    std::cerr << "Error: No samples."
              << std::endl;
    exit(1);
  }
  if (65535 < samples.size()) {
    std::cerr << "Error: Too much total number of samples: "
              << samples.size()
              << std::endl;
    exit(1);
  }

  uint16_t total_samples = samples.size();
  uint16_t loop_samples = 0;
  if (vgm_header.loop_samples) {
    loop_samples = std::ceil((double)vgm_header.loop_samples * rate / 44100);
  }

  // printf("Number of samples in total @ %5.2fHz\n", (double)rate);
  // printf("  expect: %.2f\n", (double)vgm_header.total_samples * rate / 44100);
  // printf("  actual: %zu\n", (size_t)total_samples);
  // if (loop_samples) {
  //   printf("Number of samples of loop @ %5.2fHz\n", (double)rate);
  //   printf("  expect: %.2f\n", (double)vgm_header.loop_samples * rate / 44100);
  //   printf("  actual: %zu\n", (size_t)loop_samples);
  // }

  auto compressed_samples = compress(samples);

  uint32_t scc_wav_content_size = scc_wavtbl.size() * 32;
  uint32_t scc_wav_header_size = scc_wav_content_size ? 8 : 0; // tag + content_size
  uint32_t samples_content_size = compressed_samples.size();
  uint32_t samples_header_size = samples_content_size ? 8 : 0; // tag + content_size

  uint32_t content_size
    = 1                         // rate
    + 1                         // soundchip_enable
    + sizeof(total_samples)     // total_samples
    + sizeof(loop_samples)      // loop_samples
    + 1                         // channel_enable
    + 1                         // (padding)
    + scc_wav_header_size
    + scc_wav_content_size
    + samples_header_size
    + samples_content_size
    ;

  if (content_size) {
    // file header
    write_tag(os, TAG_LA0_FILE);
    write(os, content_size);
    write(os, (uint8_t)rate);
    write(os, (uint8_t)soundchip_enable);
    write(os, total_samples);
    write(os, loop_samples);
    write(os, channel_enable);
    os << (uint8_t)0;

    // assets -------------------------------------------------
    // SCC/SCC+ waveform table
    if (scc_wav_content_size) {
      write_tag(os, TAG_SCC_WAVETBL);
      write(os, scc_wav_content_size);
      for (auto i : scc_wavtbl) {
        os.write((const char *)i.data(), i.size());
      }
    }

    // samples ------------------------------------------------
    // AY-3-8910, OPLL, and SCC/SCC+ samples
    if (samples_content_size) {
      write_tag(os, TAG_STREAM_DATA);
      write(os, samples_content_size);
      write(os, compressed_samples);
    }
  }
}
