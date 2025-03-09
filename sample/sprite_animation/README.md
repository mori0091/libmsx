# sample: `MSX2` Animated Sprite

Usecase of libmsx **SM2 (Sprite Mode 2)** Animated Color Sprite module.

https://github.com/user-attachments/assets/207c4c1f-465a-4bb0-936e-473018fcbcb0

## Requirements

- [aseprite](https://www.aseprite.org/) : Animated Sprite Editor & Pixel Art Tool
- [ase2msx](https://github/mori0091/ase2msx) : Generates sprite data and code for libmsx/MSX2 from aseprite file.

## Build Instruction

Do `make depends` if you want to re-create sprite data/code from aseprite file.
(This needs `aseprite` and `ase2msx`)
~~~ shell
make depends
~~~

Then do `make`.
~~~ shell
make
~~~

## Using Assets

- `TJ_dog.ase`  
  An aseprite sprite-sheet made and arranged for this sample application by Daishi Mori (mori0091).  

## References

libmsx SM2 tutorial:
- [TUTORIAL : SM2 (Sprite Mode 2) - Animated Color Sprites](https://github.com/mori0091/libmsx/tree/main/docs/tutorial_SM2.md)

libmsx SM2 API reference:
- [SM2 (Sprite Mode 2) Animated Color Sprites](https://mori0091.github.io/libmsx/group__SM2.html)
