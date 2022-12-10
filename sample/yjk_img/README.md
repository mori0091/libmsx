# sample: `MSX2+` YJK color, raster scroll, and resource files usecase

<https://user-images.githubusercontent.com/659805/206873387-0f5f6584-1250-4a0f-b129-44a2b617eb67.mp4>

This sample loads and shows an embedded YJK image file in MegaROM.

Such embedded files are called **resource**.

- Files in the `resources` folder are embedded into MegaROM image as
  **resource**.
- A **resource** is able to find by its name at run time.
- A **resource** is able to read / copy to RAM/VRAM.

See also `bmem.h` and `resources.h`.
