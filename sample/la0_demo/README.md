# sample: `MSX` LA0 - A decoder for the libmsx audio replayer

This sample application shows usecase of `LA0` audio decoder and the libmsx
audio replayer.

However `sample/la0_demo` cannot be built because there is no song data.

If you have some VGM files, you can try the following method:

1. Convert your VGM files to `bgm.la0` and/or `sfx.la0` using the `tools/vgm2la` tool.
2. Put the `bgm.la0` and/or `sfx.la0` in `sample/la0_demo/resources/` folder.
3. Then do `make clean all` in the `sample/la0_demo/` folder.
4. So `la0_demo.rom` file will be made in `sample/la0_demo/bin/`.
5. Confirm the ROM image size is less than or equals to 4MiB.
6. Run it on OpenMSX emulator:
~~~
openmsx -machine C-BIOS_MSX2+_JP -extb scc+ -cart bin/la0_demo.rom -romtype ascii16
~~~


> We would like to bundle some song data with `libmsx`.  
> _**If you have redistributable VGM files (`.vgm` or `.vgz`), please provide them.**_
