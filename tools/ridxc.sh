#!/bin/sh
# -*- coding: utf-8-unix -*-

list_used_banks () {
    if [ $# -gt 0 ] ; then
        cat "$@" |
            grep -E '#pragma +codeseg +BANK[0-9]+' |
            sort -n |
            uniq |
            sed -E -e 's/#pragma +codeseg +BANK([0-9]+)/\1/'
    fi
}

max_bank_of () {
    { echo 0; list_used_banks "$@"; } | tail -n 1
}

list () {
    resources_dir="$1"
    shift
    offset="$(( 16384 * ( $(max_bank_of "$@") + 1 ) ))";
    find "${resources_dir}" -type f -printf '%s\t%P\n' |
        while read s n;
        do
            printf '  {0x%08XL, %8dL, \"%s\"},\n' "${offset}" "${s}" "${n}";
            offset=$((offset + s));
        done
}

usage_and_exit ()
{
    cat 1>&2 <<EOF
Usage: $(basename "$0") [OPTION]... RESOURCE_DIR [C_SOURCE_FILE]...

Resource index compiler for libmsx.

Indexes resource files in RESOURCE_DIR and outputs them as C source files.
The optional C_SOURCE_FILE is used to determine the offset address of each
resource file in banked memory.

Options:
  -h,      --help           Display this help and exits.
  -o FILE                   Place the output into FILE.
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
    OUTPUT=;
    ARGS=;
    while [ $# -gt 0 ]
    do
        case "$1" in
            -h | --help )
                usage_and_exit;
                ;;
            -o )
                OUTPUT="$2";
                shift 2;
                ;;
            -* )
                error "Illegal option '$1'"
                ;;
            * )
                ARGS="${ARGS} $1"
                shift
                ;;
        esac
    done
}

compile ()
{
    contents="$(list "$@")"
    if [ "${contents}" ] ; then
        cat <<EOF
// -*- coding: utf-8-unix -*-";
#include <resources.h>
#include <stddef.h>

const ResourceIndex libmsx__bmem_resources[] = {
${contents}
};

const size_t libmsx__bmem_resources_length
  = sizeof(libmsx__bmem_resources)
  / sizeof(libmsx__bmem_resources[0])
  ;

EOF
    fi
}

OUTPUT=;
ARGS=;
parse_args "$@";
set -- ${ARGS}

if [ $# -lt 1 ] ; then
    error "requires a resource folder."
fi

if [ "${OUTPUT}" ] ; then
    exec > "${OUTPUT}"
fi
compile "$@"
