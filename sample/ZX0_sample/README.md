# sample: `MSX`/`MSX2`/`MSX2+`/`MSXturboR` Decompression of ZX0 compressed image

Loads / decompresses the following image files in banked memory (MegaROM) to
VRAM.

For each file, first load a RAW image in `BSAVE` format into VRAM for reference,
then decompress the `ZX0` compressed image into VRAM.

| File name (RAW) | File name (ZX0) | SCREEN | WxH (bpp)   |
|-----------------|-----------------|--------|-------------|
| KOTATU.SC2      | KOTATU.SC2.zx0  | 2      | 256x192 (2) |
| KOTATU.SC5      | KOTATU.SC5.zx0  | 5      | 256x212 (4) |
| KOTATU.SC7      | KOTATU.SC7.zx0  | 7      | 512x212 (4) |
| KOTATU.SC8      | KOTATU.SC8.zx0  | 8      | 256x212 (8) |
| KOTATU.S12      | KOTATU.S12.zx0  | 12     | 256x212 (8) |

> A `BSAVE` format file consists of a 7-byte header and RAW image data.
> 
> The `ZX0` compressed image here is the ZX0 compressed RAW image data (not
> including the 7-byte header of BSAVE format). And that is aligned to each
> 16KiB segments for faster decompression.

The C implementation of the ZX0 decompressor for **libmsx** has been
reimplemented for use with **libmsx** by Daishi Mori (mori0091).
That supports decompression of ZX0 data in ROM/RAM/MegaROM to RAM/VRAM.

And also the ZX0 file compressor tool (`tools/zx0`) has been reimplemented by
Daishi Mori (mori0091). That is ;

- ZX0 data compression format v.2 compatible.
- supports MSX-optimized ZX0 compression mode to compress MSX BSAVE format files.
  (removes the 7-byte header of BSAVE format and aligns to each 16KiB input segments)


The origins of the ZX0 data compression format and algorithm were designed and
implemented by Einar Saukas. See also <https://github.com/einar-saukas/ZX0>.


# Performance of the ZX0 decompression algorithm in libmsx

The table below shows the results of the experiment on April 1, 2023.

It shows the elapsed time (in frames at 60 Hz) required to decompress a ZX0
compressed images.

## Results of `ZX0_decompress_resource_to_vmem()` function

Decompress from ROM to VRAM directly.

| File name      | SCREEN | WxH (bpp)   | `1` | `2` | `2+` | `tR/Z80` | `tR/R800` |
|----------------|--------|-------------|-----|-----|------|----------|-----------|
| KOTATU.SC2.zx0 | 2      | 256x192 (2) | 161 | 169 | 169  | 166      | 96        |
| KOTATU.SC5.zx0 | 5      | 256x212 (4) |     | 396 | 396  | 389      | 217       |
| KOTATU.SC7.zx0 | 7      | 512x212 (4) |     | 704 | 704  | 693      | 399       |
| KOTATU.SC8.zx0 | 8      | 256x212 (8) |     | 675 | 675  | 664      | 388       |
| KOTATU.S12.zx0 | 12     | 256x212 (8) |     |     | 857  | 842      | 459       |

where

- `1`       : MSX   ; C-BIOS_MSX1_JP
- `2`       : MSX2  ; C-BIOS_MSX2_JP
- `2+`      : MSX2+ ; C-BIOS_MSX2+_JP
- `tR/Z80`  : MSXturboR / Z80 mode ; Panasonic_FS-A1GT
- `tR/R800` : MSXturboR / R800 DRAM mode ; Panasonic_FS-A1GT

## Results of `ZX0_decompress_resource_to_vmem_buffer()` function

Decompress from ROM to VRAM via main RAM page #0 (16KiB) buffer.

| File name      | SCREEN | WxH (bpp)   | `1` | `2` | `2+` | `tR/Z80` | `tR/R800` |
|----------------|--------|-------------|-----|-----|------|----------|-----------|
| KOTATU.SC2.zx0 | 2      | 256x192 (2) | 118 | 118 | 118  | 116      | 51        |
| KOTATU.SC5.zx0 | 5      | 256x212 (4) |     | 286 | 286  | 279      | 123       |
| KOTATU.SC7.zx0 | 7      | 512x212 (4) |     | 501 | 501  | 490      | 218       |
| KOTATU.SC8.zx0 | 8      | 256x212 (8) |     | 479 | 479  | 468      | 209       |
| KOTATU.S12.zx0 | 12     | 256x212 (8) |     |     | 626  | 612      | 272       |

where

- `1`       : MSX   ; C-BIOS_MSX1_JP
- `2`       : MSX2  ; C-BIOS_MSX2_JP
- `2+`      : MSX2+ ; C-BIOS_MSX2+_JP
- `tR/Z80`  : MSXturboR / Z80 mode ; Panasonic_FS-A1GT
- `tR/R800` : MSXturboR / R800 DRAM mode ; Panasonic_FS-A1GT

# Conclusion

In contrast to the RAW images, ZX0 takes more than 10 times longer to
decompress. This is probably mainly due to the low performance of VRAM access.
In practice, decompression into RAM is fast enough.

LZ77/LZSS decompression algorithms such as ZX0 require in-place copying of byte
sequences or large sliding window buffers. However, copying a byte sequence in
VRAM to another location in VRAM is the worst use case for the TMS9918, V9938,
and V9958 VDPs. It is also particularly difficult on the MSX to allocate a large
contiguous buffer in RAM.

The MSX-optimized compression mode and `ZX0_decompress_*_to_vmem_buffer()` are
designed to avoid this bottle-neck.

Of course, libmsx's implementation of the ZX0 decompressor is not yet that
optimal. Perhaps it could be made a bit faster...
