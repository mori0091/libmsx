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
        [ "Ubuntu", "index.html#autotoc_md10", null ],
        [ "Windows", "index.html#autotoc_md11", null ]
      ] ],
      [ "Build instructions of the library and sample projects", "index.html#autotoc_md12", null ],
      [ "Build instructions of user applications", "index.html#autotoc_md13", [
        [ "Preparation", "index.html#autotoc_md14", null ],
        [ "Build your application", "index.html#autotoc_md15", null ]
      ] ]
    ] ],
    [ "design-notes_crt0", "md_docs_design_notes_crt0.html", [
      [ "DESIGN NOTE : crt0 - C startup routine", "md_docs_design_notes_crt0.html#autotoc_md16", null ],
      [ "<tt>make</tt> variables for crt0 configuration", "md_docs_design_notes_crt0.html#autotoc_md17", [
        [ "CONFIG_ROM_TYPE", "md_docs_design_notes_crt0.html#autotoc_md18", null ],
        [ "CONFIG_CRT0_MOD_ROM_HEADER", "md_docs_design_notes_crt0.html#autotoc_md19", null ],
        [ "CONFIG_CRT0_MOD_INIT", "md_docs_design_notes_crt0.html#autotoc_md20", null ],
        [ "CONFIG_CRT0_MOD_START", "md_docs_design_notes_crt0.html#autotoc_md21", null ],
        [ "CONFIG_CRT0_MOD_ROM_MAPPER", "md_docs_design_notes_crt0.html#autotoc_md22", null ],
        [ "CONFIG_CRT0_MOD_LIBS", "md_docs_design_notes_crt0.html#autotoc_md23", null ],
        [ "USE_ALL_EXTENSIONS (deprecated)", "md_docs_design_notes_crt0.html#autotoc_md24", null ]
      ] ],
      [ "Pre-defined crt0 configurations", "md_docs_design_notes_crt0.html#autotoc_md25", [
        [ "${LIBMSX_HOME}/mk/16k.4000.mk", "md_docs_design_notes_crt0.html#autotoc_md26", null ],
        [ "${LIBMSX_HOME}/mk/32k.4000.mk", "md_docs_design_notes_crt0.html#autotoc_md27", null ],
        [ "${LIBMSX_HOME}/mk/ascii8.mk", "md_docs_design_notes_crt0.html#autotoc_md28", null ],
        [ "${LIBMSX_HOME}/mk/ascii16.mk", "md_docs_design_notes_crt0.html#autotoc_md29", null ]
      ] ]
    ] ],
    [ "DESIGN NOTE : SNDDRV - a sound driver", "md_docs_design_notes_snddrv.html", [
      [ "About", "md_docs_design_notes_snddrv.html#autotoc_md31", null ],
      [ "Features", "md_docs_design_notes_snddrv.html#autotoc_md32", null ],
      [ "How to integrate Arkos Tracker 2 .aks files into your application", "md_docs_design_notes_snddrv.html#autotoc_md33", null ]
    ] ],
    [ "DESIGN NOTE : Internal of SNDDRV", "md_docs_design_notes_snddrv_internal.html", [
      [ "SNDDRV's sound processing pipiline", "md_docs_design_notes_snddrv_internal.html#autotoc_md35", null ],
      [ "Sound data structure", "md_docs_design_notes_snddrv_internal.html#autotoc_md36", null ],
      [ "Sound data format notation", "md_docs_design_notes_snddrv_internal.html#autotoc_md37", null ],
      [ "Definition of Sound data", "md_docs_design_notes_snddrv_internal.html#autotoc_md38", [
        [ "Definition of SpeedTrack stream", "md_docs_design_notes_snddrv_internal.html#autotoc_md39", null ],
        [ "Definition of EventTrack stream", "md_docs_design_notes_snddrv_internal.html#autotoc_md40", null ],
        [ "Definition of Track stream", "md_docs_design_notes_snddrv_internal.html#autotoc_md41", [
          [ "Track stream", "md_docs_design_notes_snddrv_internal.html#autotoc_md42", null ],
          [ "Chunk of Track stream", "md_docs_design_notes_snddrv_internal.html#autotoc_md43", null ]
        ] ],
        [ "Definition of Pattern table", "md_docs_design_notes_snddrv_internal.html#autotoc_md44", null ]
      ] ],
      [ "Definition of Instrument (timbre) table", "md_docs_design_notes_snddrv_internal.html#autotoc_md45", null ]
    ] ],
    [ "Todo List", "todo.html", null ],
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
        [ "Functions", "globals_func.html", null ],
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
"annotated.html",
"snd__sound_8h.html#ab8ce04d6fbc22adce69bcc28aaf92327",
"vdp_8h.html#afdffcc91a0792402633458501bdb287a"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';