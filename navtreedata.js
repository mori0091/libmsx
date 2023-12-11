/*
 @licstart  The following is the entire license notice for the JavaScript code in this file.

 The MIT License (MIT)

 Copyright (C) 1997-2020 by Dimitri van Heesch

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or
 substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 @licend  The above is the entire license notice for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "libmsx", "index.html", [
    [ "libmsx ; C library for MSX", "index.html", [
      [ "LICENSE", "index.html#autotoc_md1", null ],
      [ "API reference", "index.html#autotoc_md2", null ],
      [ "Supported functionality", "index.html#autotoc_md3", null ],
      [ "Unsupported functionality", "index.html#autotoc_md4", null ],
      [ "Pre-requisites", "index.html#autotoc_md5", [
        [ "Target Machine", "index.html#autotoc_md6", null ],
        [ "Host Platform", "index.html#autotoc_md7", null ],
        [ "Build tools", "index.html#autotoc_md8", null ]
      ] ],
      [ "Install instructions of Build tools", "index.html#autotoc_md9", [
        [ "Ubuntu 22.04", "index.html#autotoc_md10", [
          [ "1. Install SDCC", "index.html#autotoc_md11", null ],
          [ "2. Install other toolchain", "index.html#autotoc_md12", null ]
        ] ],
        [ "Windows (Ubuntu on WSL2)", "index.html#autotoc_md13", null ],
        [ "Windows (MSYS2)", "index.html#autotoc_md14", [
          [ "1. Install SDCC", "index.html#autotoc_md15", null ],
          [ "2. Install other toolchain", "index.html#autotoc_md16", null ]
        ] ]
      ] ],
      [ "Build instructions of the library and sample projects", "index.html#autotoc_md17", null ],
      [ "Build instructions of user applications", "index.html#autotoc_md18", [
        [ "Preparation", "index.html#autotoc_md19", null ],
        [ "Build your application", "index.html#autotoc_md20", null ]
      ] ]
    ] ],
    [ "DESIGN NOTE : crt0 - C startup routine", "md_docs_design_notes_crt0.html", [
      [ "Make-variables for crt0 configuration", "md_docs_design_notes_crt0.html#autotoc_md22", [
        [ "CONFIG_ROM_TYPE", "md_docs_design_notes_crt0.html#autotoc_md23", null ],
        [ "CONFIG_CRT0_MOD_ROM_HEADER", "md_docs_design_notes_crt0.html#autotoc_md24", null ],
        [ "CONFIG_CRT0_MOD_INIT", "md_docs_design_notes_crt0.html#autotoc_md25", null ],
        [ "CONFIG_CRT0_MOD_START", "md_docs_design_notes_crt0.html#autotoc_md26", null ],
        [ "CONFIG_CRT0_MOD_ROM_MAPPER", "md_docs_design_notes_crt0.html#autotoc_md27", null ],
        [ "CONFIG_CRT0_MOD_LIBS", "md_docs_design_notes_crt0.html#autotoc_md28", null ],
        [ "USE_ALL_EXTENSIONS (deprecated)", "md_docs_design_notes_crt0.html#autotoc_md29", null ]
      ] ],
      [ "Pre-defined crt0 configurations", "md_docs_design_notes_crt0.html#autotoc_md30", [
        [ "${LIBMSX_HOME}/mk/16k.4000.mk", "md_docs_design_notes_crt0.html#autotoc_md31", null ],
        [ "${LIBMSX_HOME}/mk/32k.4000.mk", "md_docs_design_notes_crt0.html#autotoc_md32", null ],
        [ "${LIBMSX_HOME}/mk/ascii8.mk", "md_docs_design_notes_crt0.html#autotoc_md33", null ],
        [ "${LIBMSX_HOME}/mk/ascii16.mk", "md_docs_design_notes_crt0.html#autotoc_md34", null ]
      ] ]
    ] ],
    [ "DESIGN NOTE : SNDDRV - a sound driver", "md_docs_design_notes_snddrv.html", [
      [ "About", "md_docs_design_notes_snddrv.html#autotoc_md36", null ],
      [ "Features", "md_docs_design_notes_snddrv.html#autotoc_md37", null ],
      [ "How to integrate Arkos Tracker 2 .aks files into your application", "md_docs_design_notes_snddrv.html#autotoc_md38", null ]
    ] ],
    [ "DESIGN NOTE : Internal of SNDDRV", "md_docs_design_notes_snddrv_internal.html", [
      [ "SNDDRV's sound processing pipiline", "md_docs_design_notes_snddrv_internal.html#autotoc_md40", null ],
      [ "Sound data structure", "md_docs_design_notes_snddrv_internal.html#autotoc_md41", null ],
      [ "Sound data format notation", "md_docs_design_notes_snddrv_internal.html#autotoc_md42", null ],
      [ "Definition of Sound data", "md_docs_design_notes_snddrv_internal.html#autotoc_md43", [
        [ "Definition of SpeedTrack stream", "md_docs_design_notes_snddrv_internal.html#autotoc_md44", null ],
        [ "Definition of EventTrack stream", "md_docs_design_notes_snddrv_internal.html#autotoc_md45", null ],
        [ "Definition of Track stream", "md_docs_design_notes_snddrv_internal.html#autotoc_md46", [
          [ "Track stream", "md_docs_design_notes_snddrv_internal.html#autotoc_md47", null ],
          [ "Chunk of Track stream", "md_docs_design_notes_snddrv_internal.html#autotoc_md48", null ]
        ] ],
        [ "Definition of Pattern table", "md_docs_design_notes_snddrv_internal.html#autotoc_md49", null ]
      ] ],
      [ "Definition of Instrument (timbre) table", "md_docs_design_notes_snddrv_internal.html#autotoc_md50", null ]
    ] ],
    [ "Modules", "modules.html", "modules" ],
    [ "Data Structures", "annotated.html", [
      [ "Data Structures", "annotated.html", "annotated_dup" ],
      [ "Data Structure Index", "classes.html", null ],
      [ "Data Fields", "functions.html", [
        [ "All", "functions.html", null ],
        [ "Functions", "functions_func.html", null ],
        [ "Variables", "functions_vars.html", null ]
      ] ]
    ] ],
    [ "Files", "files.html", [
      [ "File List", "files.html", "files_dup" ],
      [ "Globals", "globals.html", [
        [ "All", "globals.html", "globals_dup" ],
        [ "Functions", "globals_func.html", "globals_func" ],
        [ "Variables", "globals_vars.html", null ],
        [ "Typedefs", "globals_type.html", null ],
        [ "Enumerations", "globals_enum.html", null ],
        [ "Enumerator", "globals_eval.html", null ],
        [ "Macros", "globals_defs.html", null ]
      ] ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"ZX0__decompress_8h.html",
"group__SCC__PREDEFINED__WAVEFORMS.html#ga80d2c9d452e67cfdfdce68650cea0dc9",
"group__VDP__COMMANDS__HIGH.html#gaa54275abbd2308fcc53342062d9086f1",
"snd__p__table_8h.html"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';