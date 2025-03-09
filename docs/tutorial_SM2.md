# TUTORIAL : SM2 (Sprite Mode 2) - Animated Color Sprites.

SM2 module provides APIs for displaying animated multicolor sprites.

# Requirements

SM2 is designed to use with :
- [aseprite](https://www.aseprite.org/) ; Animated Sprite Editor & Pixel Art Tool
- [ase2msx](https://github.com/mori0091/ase2msx) ; Generates sprite data and code for libmsx/MSX2 from aseprite file.

These must be installed and path to installed folder must be added to `PATH`
environment variable.

# Tutorial

## Step 1. Make/Edit your sprite sheet with aseprite

   Requirements:
   - The sprite size must be multiple of 16x16 pixels.
   - IndexColor with up to 16 colors (15 colors + 1 transparent).
   - Color code #0 is transparent.

   In the steps that follow, assume the following:
   - The aseprite file is saved under the name `MySprite.ase`.
   - The file has two tags, `Idle` and `Walk`.

## Step 2. Generate sprite data and code from the aseprite file

   ~~~ shell
   ase2msx MySprite.ase
   ~~~

   That generates the following files:
   - `MySprite.json` : exported by aseprite
   - `MySprite.png`  : exported by aseprite
   - `MySprite.c`    : C source code
   - `MySprite.h`    : C header file
   - `MySprite.sm2`  : Sprite data

## Step 3. Import generated data and code into your application's repository

   ~~~ shell
   mv MySprite.c MySprite.h src/
   mv MySprite.sm2 resources/
   ~~~

## Step 4. Write Makefile for building your application

   ~~~ Makefile
   # Path to top of the libmsx installed folder.
   LIBMSX_HOME = path_to_top_of_libmsx

   # Specify NAME of your rom image to be built.
   NAME = name_of_your_rom

   # Select ROM type.
   CONFIG_ROM_TYPE = ascii16

   # Include common part of Makefile
   include ${LIBMSX_HOME}/mk/build.mk
   ~~~

   Note that the `CONFIG_ROM_TYPE` must be `ascii8` or `ascii16`.

   See also [Makefile template](https://github.com/mori0091/libmsx/blob/main/mk/Makefile)

## Step 5. Write your application code which use the generated code and SM2 APIs

   ~~~ c
   // \file src/main.c

   #include <msx.h>
   #include <screen.h>

   // 👉 Include generated header file.
   #include "MySprite.h"

   // Color palette
   static palette_t palette[16] = {RGB(0,0,0), /* snip */, RGB(7,7,7)};

   // Sprite objects
   static sm2_Sprite sprites[2];

   void perform_animation_step(void) {
     // Calculate the remaining time of the current animation frame and 
     // update the frame number.
     sm2_update_sprites(sprites, sizeof(sprites)/sizeof(sprites[0]));
     // \note
     // To perform an animation step,
     // This must be called at each VSYNC timing.
   }

   void main(void) {
     screen5();

     // The color palette must be initialized manually.
     // (The palette data is not included in the sprite sheet data file.)
     vdp_write_palette(palette);

     // VDP register setting for sprites
     // - Sprite size (16x16 or 16x16_MAGNIFIED)
     vdp_set_sprite_size(VDP_SPRITE_SIZE_16x16);

     // Initialize SM2 module.
     sm2_init();

     // 👉 Initialize sprite sheet.
     MySprite_init();

     // Construct/Initialize animated sprites.
     sm2_init_sprite(&sprites[0], &MySprite, &MySprite_tag_Idle);
     sm2_init_sprite(&sprites[1], &MySprite, &MySprite_tag_Walk);

     // Perform an animation step at each VSYNC timing.
     set_vsync_handler(perform_animation_step);

     // Main loop
     for (;;) {
       // Add some sprites to the internal list.
       sm2_add_sprite(&sprites[0], 0, 0);
       sm2_add_sprite(&sprites[1], 0, 32);
       // Next, the list is flushed to update the displayed sprites.
       sm2_flush();
     } // Repeat the above as needed and/or periodically.
   }
   ~~~

## Step 6. Build your application with the generated data and code

   Now your application repository shall be as follows:
   ~~~
   ./
   ├── Makefile
   ├── resources/
   │   └── MySprite.sm2
   └── src/
       ├── MySprite.c
       ├── MySprite.h
       └── main.c
   ~~~

   To build your application, run `make`.
   ~~~ shell
   make
   ~~~

# References

See also the SM2 API reference:
- [SM2 (Sprite Mode 2) Animated Color Sprites](https://mori0091.github.io/libmsx/group__SM2.html)

Application example:
- [Usecase of libmsx SM2 (Sprite Mode 2) Animated Color Sprite module](https://github.com/mori0091/libmsx/tree/main/sample/sprite_animation)
