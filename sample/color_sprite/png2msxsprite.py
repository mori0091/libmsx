#!/usr/bin/env python3
# -*- coding: utf-8-unix -*-

# Copyright (c) 2021-2025 Daishi Mori (mori0091)
#
# This software is released under the MIT License.
# See https://github.com/mori0091/libmsx/blob/main/LICENSE
#
# GitHub libmsx project
# https://github.com/mori0091/libmsx

# Usage
# =====
# ```shell
# python3 png2msxsprite.py A.png
# ```
# Creates sprite data from A.png.
#
#  sprite pattern table  : A_spt.bin
#  sprite color table    : A_sct.bin
#  sprite attribute table: A_sat.bin
#      (for 2x magnified): A_sat2x.bin
#
# The PNG must be
# - Multiple of 16x16 pixels
# - IndexColor w/ up to 16 colors
# - color code #0 is transparent

# To use this python script, PIL and NumPy are required:
# ```shell
# sudo apt install python3-pil python3-pil.imagetk
# sudo apt install python3-numpy
# ```

import os
import sys
import numpy as np

from PIL import Image

def filter(a, c):
    c = np.tile(c, (16,1)).T
    return (((a & c) == c) & (c != 0)).astype(int)

# Encode 16x16 1bpp image to sprite pattern data.
# \param a 16x16 ndarray, 1bpp
# \return 1x32 ndarray
def encode_16x16x1bpp(a):
    v = np.array([128, 64, 32, 16, 8, 4, 2, 1]).reshape((8,1))
    m1 = (a.reshape(32,8) @ v).reshape(16,2).T.flatten()
    return m1

# Encode 16x16 1layer image to sprite pattern data.
# \param a 16x16 ndarray
# \param c color (1x16 ndarray)
# \return 1x32 ndarray
def encode_16x16x1(a, c):
    return encode_16x16x1bpp(filter(a, c))

# Encode 16x16 2layer image to 2 layer sprite pattern data.
# \param a 16x16 ndarray
# \param c 2 layer colors (2x16 ndarray)
# \return 2x32 ndarray
def encode_16x16x2(a, c):
    c1, c2 = c
    m1 = encode_16x16x1(a, c1)
    m2 = encode_16x16x1(a, c2)
    return np.vstack((m1,m2))

# Encode 16x16 3layer image to 3 layer sprite pattern data.
# \param a 16x16 ndarray
# \param c 3 layer colors (3x16 ndarray)
# \return 3x32 ndarray
def encode_16x16x3(a, c):
    c1, c2, c3 = c
    m1 = encode_16x16x1(a, c1)
    m2 = encode_16x16x1(a, c2)
    m4 = encode_16x16x1(a, c3)
    return np.vstack((m1,m2,m4))

# Encode 16x16 4layer image to 4 layer sprite pattern data.
# \param a 16x16 ndarray
# \param c 4 layer colors (4x16 ndarray)
# \return 4x32 ndarray
def encode_16x16x4(a, c):
    c1, c2, c3, c4 = c
    m1 = encode_16x16x1(a, c1)
    m2 = encode_16x16x1(a, c2)
    m4 = encode_16x16x1(a, c3)
    m8 = encode_16x16x1(a, c4)
    return np.vstack((m1,m2,m4,m8))

##########################################################################

# Constructs color table up to 4 layers.
# \param a 16x16 ndarray
# \return Nx16 ndarray (N <= 4)
def color_table(a):
    s = np.sort(a)              # sort for each rows
    c = np.empty((0,0))
    for i in range(0, s.shape[0]):
        v = np.unique(s[i])
        v = color_combination(v[0<v])
        if v.size == 0:
            v = np.zeros((4), dtype=int)
        elif v.size < 4:
            v = np.hstack((np.array(v), np.zeros((4-v.size), dtype=int)))
        if c.size == 0:
            c = v
        else:
            c = np.vstack((c, v))
    c = c.T
    if np.all(c[1:,] == 0):
        return c[0:1,]
    elif np.all(c[2:,] == 0):
        return c[0:2,]
    elif np.all(c[3:,] == 0):
        return c[0:3,]
    else:
        return c

def color_combination(v):
    if v.size <= 2:
        return v
    elif 8 <= v.size:
        return np.array([1,2,4,8])
    else:
        return np.array(find_minimal_set(v))

def find_minimal_set(X):
    from itertools import combinations

    # Define the universe set U
    U = set(range(1, max(X)+1))

    # Function to compute the logical OR sum of a set
    def logical_or_sum(subset):
        a = 0
        for i in subset:
            a = a | i
        return a

    # Initialize the minimum size of Y to infinity
    min_size = float('inf')
    minimal_set = None

    # Iterate over all possible subsets of U that could be Y
    for r in range(1, min({len(U), 4}) + 1):
        for combination in combinations(U, r):
            # Check if this combination can generate X through logical OR operations
            generated_X = set()
            for x in X:
                # Find a subset of combinations whose logical OR is x
                for i in range(1, len(combination) + 1):
                    for subset in combinations(combination, i):
                        if logical_or_sum(subset) == x:
                            generated_X.add(x)
                            break
                    if x in generated_X:
                        break

            # If we can generate all elements of X, we check if this is the smallest Y
            if generated_X == set(X):
                if len(combination) < min_size:
                    min_size = len(combination)
                    minimal_set = set(combination)

    return sorted(minimal_set)

##########################################################################

# Encode 16x16 1..4bpp image to 1..4 layer sprite pattern/color data.
# \return
#   - sprite pattern table as Nx32 ndarray
#   - sprite color table as Nx16 ndarray
def encode_16x16(a):
    CC = (1 << 6)               # CC bit of sprite color table
    sct = color_table(a)
    num_layers = sct.shape[0]
    if num_layers == 1:
        spt = encode_16x16x1(a, sct)
        return (spt, sct)
    elif num_layers == 2:
        spt = encode_16x16x2(a, sct)
        sct[1:,] = sct[1:,] | CC
        sct[sct == CC] = 0
        return (spt, sct)
    elif num_layers == 3:
        spt = encode_16x16x3(a, sct)
        sct[1:,] = sct[1:,] | CC
        sct[sct == CC] = 0
        return (spt, sct)
    elif num_layers == 4:
        spt = encode_16x16x4(a, sct)
        sct[1:,] = sct[1:,] | CC
        sct[sct == CC] = 0
        return (spt, sct)
    else:
        sys.exit('What\'s happen!?')

##########################################################################

np.set_printoptions(threshold=np.inf, linewidth=84)

path = sys.argv[1]
file, ext = os.path.splitext(path)
with Image.open(path) as im:
    # print(im.format, im.size, im.mode)
    assert im.mode == 'P' and len(im.getcolors()) <= 16, \
        'The image must be IndexColor up to 16 colors'
    assert im.width % 16 == 0 and im.height % 16 == 0, \
        'Width and height of the image must be multiple of 16'

    palette = im.getpalette()
    palette = np.array(palette).reshape((len(palette)//3,3))

    a = np.array(im)
    height, width = a.shape

    np.set_printoptions(formatter={'int': '{:X}'.format})
    print(a)

    np.set_printoptions(formatter={'int': '{:3d}'.format})
    print()
    print('# color palette')
    print(palette)

    np.set_printoptions(formatter={'int': '0x{:02X}'.format})
    idx = 0
    k = 0
    sats2x = None
    sats = None
    spts = None
    scts = None
    for y in range(0, height, 16):
        for x in range(0, width, 16):
            crop = a[y:y+16,x:x+16]
            spt, sct = encode_16x16(crop)
            n = spt.shape[0]
            pats = [i*4 for i in range(k, k+n)]
            sat = np.array([(y%256, x%256, p, 0) for p in pats])
            sat2x = np.array([(y*2%256, x*2%256, p, 0) for p in pats])
            k = k + n
            print()
            print('# cropped area [x y width height] =', '[{} {} 16 16]'.format(x,y))
            print('# pattern numbers =', pats)
            print('# sprite attribute table [y x pat _]')
            print(sat)
            print('# sprite pattern table')
            print(spt)
            print('# sprite color table')
            print(sct)
            if idx == 0:
                sats2x = sat2x
                sats = sat
                spts = spt
                scts = sct
            else:
                sats2x = np.vstack((sats2x, sat2x))
                sats = np.vstack((sats, sat))
                spts = np.vstack((spts, spt))
                scts = np.vstack((scts, sct))
            idx = idx + 1

    sats2x.astype(np.uint8).tofile(file + '_sat2x.bin')
    sats.astype(np.uint8).tofile(file + '_sat.bin')
    spts.astype(np.uint8).tofile(file + '_spt.bin')
    scts.astype(np.uint8).tofile(file + '_sct.bin')
