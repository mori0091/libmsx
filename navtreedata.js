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
    [ "libmsx ; C library for MSX", "index.html", "index" ],
    [ "DESIGN NOTE : crt0 - C startup routine", "md_docs_2design-notes__crt0.html", [
      [ "Make-variables for crt0 configuration", "md_docs_2design-notes__crt0.html#autotoc_md21", [
        [ "CONFIG_ROM_TYPE", "md_docs_2design-notes__crt0.html#autotoc_md22", null ],
        [ "CONFIG_CRT0_MOD_ROM_HEADER", "md_docs_2design-notes__crt0.html#autotoc_md23", null ],
        [ "CONFIG_CRT0_MOD_INIT", "md_docs_2design-notes__crt0.html#autotoc_md24", null ],
        [ "CONFIG_CRT0_MOD_START", "md_docs_2design-notes__crt0.html#autotoc_md25", null ],
        [ "CONFIG_CRT0_MOD_ROM_MAPPER", "md_docs_2design-notes__crt0.html#autotoc_md26", null ],
        [ "CONFIG_CRT0_MOD_LIBS", "md_docs_2design-notes__crt0.html#autotoc_md27", null ],
        [ "USE_ALL_EXTENSIONS (deprecated)", "md_docs_2design-notes__crt0.html#autotoc_md28", null ]
      ] ],
      [ "Pre-defined crt0 configurations", "md_docs_2design-notes__crt0.html#autotoc_md29", [
        [ "${LIBMSX_HOME}/mk/16k.4000.mk", "md_docs_2design-notes__crt0.html#autotoc_md30", null ],
        [ "${LIBMSX_HOME}/mk/32k.4000.mk", "md_docs_2design-notes__crt0.html#autotoc_md31", null ],
        [ "${LIBMSX_HOME}/mk/ascii8.mk", "md_docs_2design-notes__crt0.html#autotoc_md32", null ],
        [ "${LIBMSX_HOME}/mk/ascii16.mk", "md_docs_2design-notes__crt0.html#autotoc_md33", null ]
      ] ]
    ] ],
    [ "DESIGN NOTE : LA0 - The libmsx audio format 0.", "md_docs_2design-notes__la0.html", [
      [ "Data format notation", "md_docs_2design-notes__la0.html#autotoc_md35", null ],
      [ "LA0 file/data format", "md_docs_2design-notes__la0.html#autotoc_md36", [
        [ "STREAM_INFO", "md_docs_2design-notes__la0.html#autotoc_md37", null ],
        [ "SCC_WAVE_TABLE ; SCC/SCC+ waveform table", "md_docs_2design-notes__la0.html#autotoc_md38", null ],
        [ "STREAM_DATA", "md_docs_2design-notes__la0.html#autotoc_md39", [
          [ "SEQUENCE", "md_docs_2design-notes__la0.html#autotoc_md40", null ],
          [ "DICTIONARY", "md_docs_2design-notes__la0.html#autotoc_md41", null ]
        ] ]
      ] ],
      [ "References", "md_docs_2design-notes__la0.html#autotoc_md42", null ]
    ] ],
    [ "DESIGN NOTE : SNDDRV - a sound driver", "md_docs_2design-notes__snddrv.html", [
      [ "About", "md_docs_2design-notes__snddrv.html#autotoc_md44", null ],
      [ "Features", "md_docs_2design-notes__snddrv.html#autotoc_md45", null ],
      [ "How to integrate Arkos Tracker 2 .aks files into your application", "md_docs_2design-notes__snddrv.html#autotoc_md46", null ]
    ] ],
    [ "DESIGN NOTE : Internal of SNDDRV", "md_docs_2design-notes__snddrv__internal.html", [
      [ "SNDDRV's sound processing pipiline", "md_docs_2design-notes__snddrv__internal.html#autotoc_md48", null ],
      [ "Sound data structure", "md_docs_2design-notes__snddrv__internal.html#autotoc_md49", null ],
      [ "Sound data format notation", "md_docs_2design-notes__snddrv__internal.html#autotoc_md50", null ],
      [ "Definition of Sound data", "md_docs_2design-notes__snddrv__internal.html#autotoc_md51", [
        [ "Definition of SpeedTrack stream", "md_docs_2design-notes__snddrv__internal.html#autotoc_md52", null ],
        [ "Definition of EventTrack stream", "md_docs_2design-notes__snddrv__internal.html#autotoc_md53", null ],
        [ "Definition of Track stream", "md_docs_2design-notes__snddrv__internal.html#autotoc_md54", [
          [ "Track stream", "md_docs_2design-notes__snddrv__internal.html#autotoc_md55", null ],
          [ "Chunk of Track stream", "md_docs_2design-notes__snddrv__internal.html#autotoc_md56", null ]
        ] ],
        [ "Definition of Pattern table", "md_docs_2design-notes__snddrv__internal.html#autotoc_md57", null ]
      ] ],
      [ "Definition of Instrument (timbre) table", "md_docs_2design-notes__snddrv__internal.html#autotoc_md58", null ]
    ] ],
    [ "TUTORIAL : SM2 (Sprite Mode 2) - Animated Color Sprites.", "md_docs_2tutorial__SM2.html", [
      [ "Requirements", "md_docs_2tutorial__SM2.html#autotoc_md60", null ],
      [ "Tutorial", "md_docs_2tutorial__SM2.html#autotoc_md61", [
        [ "Step 1. Make/Edit your sprite sheet with aseprite", "md_docs_2tutorial__SM2.html#autotoc_md62", null ],
        [ "Step 2. Generate sprite data and code from the aseprite file", "md_docs_2tutorial__SM2.html#autotoc_md63", null ],
        [ "Step 3. Import generated data and code into your application's repository", "md_docs_2tutorial__SM2.html#autotoc_md64", null ],
        [ "Step 4. Write Makefile for building your application", "md_docs_2tutorial__SM2.html#autotoc_md65", null ],
        [ "Step 5. Write your application code which use the generated code and SM2 APIs", "md_docs_2tutorial__SM2.html#autotoc_md66", null ],
        [ "Step 6. Build your application with the generated data and code", "md_docs_2tutorial__SM2.html#autotoc_md67", null ]
      ] ],
      [ "References", "md_docs_2tutorial__SM2.html#autotoc_md68", null ]
    ] ],
    [ "Deprecated List", "deprecated.html", null ],
    [ "Topics", "topics.html", "topics" ],
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
"NDP_8h.html",
"group__IM__TILEMAP__BM8x8.html#ga5901522cb37c02daaac7e181034447b6",
"group__SCC__PREDEFINED__WAVEFORMS.html#gaad149b4daa1883848d3630552cb22c4e",
"group__SPRITES__ATTR.html#structsprite",
"index.html#autotoc_md12",
"workarea_8h.html#af0308ee928d41382ca763102fd14b16f"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';