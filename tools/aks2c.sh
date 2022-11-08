#!/bin/sh
# -*- coding: utf-8-unix -*-

usage_and_exit ()
{
    cat 1>&2 <<EOF
Usage: $(basename "$0") [OPTION]... file

Arakos Tracker 2 song file (.aks) to C source file translater for SNDDRV.
The SNDDRV is a PSG sound driver library, and is a part of libmsx.

Options:
  -h,      --help           Display this help and exits.
  -o FILE                   Place the output into FILE.
  -n ARG,  --name ARG       Give ARG as the variable name of snd_SoundAssets type.
           --header         Output C header file.
EOF
    exit 1
}

error ()
{
    echo "$@" 1>&2;
    exit 2;
}

parse_args ()
{
    while [ $# -gt 0 ]
    do
        case "$1" in
            -h | --help )
                usage_and_exit;
                ;;
            -o )
                XSLTPROC_FLAGS="${XSLTPROC_FLAGS} -o $2"
                shift 2;
                ;;
            -n | --name )
                XSLTPROC_FLAGS="${XSLTPROC_FLAGS} --stringparam soundAssets $2"
                shift 2;
                ;;
            --header )
                XSLTPROC_FLAGS="${XSLTPROC_FLAGS} --stringparam output_mode h"
                shift;
                ;;
            -* )
                error "Illegal option '$1'"
                ;;
            * )
                if [ "${INPUT}" ] ; then
                    error "Too many files."
                fi
                if [ ! -e "$1" ] ; then
                    error "File not found -- '$1'"
                fi
                INPUT="$1";
                shift
                ;;
        esac
    done
    if [ ! "${INPUT}" ] ; then
        error "No input file is given."
    fi
}

require ()
{
    for cmd in "$@" ; do
        which "${cmd}" 2>/dev/null 1>&2 || error "'${cmd}' command not found."
    done
}

INPUT=;
XSLTPROC_FLAGS=;

XSLT="$(dirname "$0")/aks2c.xsl"

require zcat xsltproc
parse_args "$@"
zcat "${INPUT}" | xsltproc ${XSLTPROC_FLAGS} --stringparam source_filename "${INPUT}" "${XSLT}" -
