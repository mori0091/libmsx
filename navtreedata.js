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
      [ "Make-variables for crt0 configuration", "md_docs_2design-notes__crt0.html#autotoc_md22", [
        [ "CONFIG_ROM_TYPE", "md_docs_2design-notes__crt0.html#autotoc_md23", null ],
        [ "CONFIG_CRT0_MOD_ROM_HEADER", "md_docs_2design-notes__crt0.html#autotoc_md24", null ],
        [ "CONFIG_CRT0_MOD_INIT", "md_docs_2design-notes__crt0.html#autotoc_md25", null ],
        [ "CONFIG_CRT0_MOD_START", "md_docs_2design-notes__crt0.html#autotoc_md26", null ],
        [ "CONFIG_CRT0_MOD_ROM_MAPPER", "md_docs_2design-notes__crt0.html#autotoc_md27", null ],
        [ "CONFIG_CRT0_MOD_LIBS", "md_docs_2design-notes__crt0.html#autotoc_md28", null ],
        [ "USE_ALL_EXTENSIONS (deprecated)", "md_docs_2design-notes__crt0.html#autotoc_md29", null ]
      ] ],
      [ "Pre-defined crt0 configurations", "md_docs_2design-notes__crt0.html#autotoc_md30", [
        [ "${LIBMSX_HOME}/mk/16k.4000.mk", "md_docs_2design-notes__crt0.html#autotoc_md31", null ],
        [ "${LIBMSX_HOME}/mk/32k.4000.mk", "md_docs_2design-notes__crt0.html#autotoc_md32", null ],
        [ "${LIBMSX_HOME}/mk/ascii8.mk", "md_docs_2design-notes__crt0.html#autotoc_md33", null ],
        [ "${LIBMSX_HOME}/mk/ascii16.mk", "md_docs_2design-notes__crt0.html#autotoc_md34", null ]
      ] ]
    ] ],
    [ "DESIGN NOTE : LA0 - The libmsx audio format 0.", "md_docs_2design-notes__la0.html", [
      [ "Data format notation", "md_docs_2design-notes__la0.html#autotoc_md36", null ],
      [ "LA0 file/data format", "md_docs_2design-notes__la0.html#autotoc_md37", [
        [ "STREAM_INFO", "md_docs_2design-notes__la0.html#autotoc_md38", null ],
        [ "SCC_WAVE_TABLE ; SCC/SCC+ waveform table", "md_docs_2design-notes__la0.html#autotoc_md39", null ],
        [ "STREAM_DATA", "md_docs_2design-notes__la0.html#autotoc_md40", [
          [ "SEQUENCE", "md_docs_2design-notes__la0.html#autotoc_md41", null ],
          [ "DICTIONARY", "md_docs_2design-notes__la0.html#autotoc_md42", null ]
        ] ]
      ] ],
      [ "References", "md_docs_2design-notes__la0.html#autotoc_md43", null ]
    ] ],
    [ "DESIGN NOTE : SNDDRV - a sound driver", "md_docs_2design-notes__snddrv.html", [
      [ "About", "md_docs_2design-notes__snddrv.html#autotoc_md45", null ],
      [ "Features", "md_docs_2design-notes__snddrv.html#autotoc_md46", null ],
      [ "How to integrate Arkos Tracker 2 .aks files into your application", "md_docs_2design-notes__snddrv.html#autotoc_md47", null ]
    ] ],
    [ "DESIGN NOTE : Internal of SNDDRV", "md_docs_2design-notes__snddrv__internal.html", [
      [ "SNDDRV's sound processing pipiline", "md_docs_2design-notes__snddrv__internal.html#autotoc_md49", null ],
      [ "Sound data structure", "md_docs_2design-notes__snddrv__internal.html#autotoc_md50", null ],
      [ "Sound data format notation", "md_docs_2design-notes__snddrv__internal.html#autotoc_md51", null ],
      [ "Definition of Sound data", "md_docs_2design-notes__snddrv__internal.html#autotoc_md52", [
        [ "Definition of SpeedTrack stream", "md_docs_2design-notes__snddrv__internal.html#autotoc_md53", null ],
        [ "Definition of EventTrack stream", "md_docs_2design-notes__snddrv__internal.html#autotoc_md54", null ],
        [ "Definition of Track stream", "md_docs_2design-notes__snddrv__internal.html#autotoc_md55", [
          [ "Track stream", "md_docs_2design-notes__snddrv__internal.html#autotoc_md56", null ],
          [ "Chunk of Track stream", "md_docs_2design-notes__snddrv__internal.html#autotoc_md57", null ]
        ] ],
        [ "Definition of Pattern table", "md_docs_2design-notes__snddrv__internal.html#autotoc_md58", null ]
      ] ],
      [ "Definition of Instrument (timbre) table", "md_docs_2design-notes__snddrv__internal.html#autotoc_md59", null ]
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
"ZX0__decompress_8h.html",
"group__IM__TILEMAP__BM8x8.html#ggafa2a1d17209d9f2522669013acdc9245ad39ed29b3f062883126eafed5445fb92",
"group__SLOT__INSPECT.html#ga70120a1fc1222cb231bbfb42897c0162",
"group__VDP__COMMANDS__LOW.html#ae5f7e7185ca8d8d37ff830b142bae2e7",
"structTTY__Device.html"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';