#!/bin/sh -e
# -*- coding: utf-8-unix -*-

list_used_banks () {
    if [ $# -gt 0 ] ; then
        sed -n -E -e 's/^#[[:space:]]*pragma[[:space:]]+codeseg[[:space:]]+BANK([0-9]+)/\1/p' "$@" |
            sort -n -u
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
        while read -r s n;
        do
            printf '  {0x%08XUL, %8dUL, "%s"},\n' "${offset}" "${s}" "${n}";
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
                if [ $# -lt 2 ] ; then
                    error "Argument required: '-o'"
                fi
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
#pragma codeseg CODE
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
eval "set -- ${ARGS}"

if [ $# -lt 1 ] ; then
    error "requires a resource folder."
fi

if [ "${OUTPUT}" ] ; then
    exec > "${OUTPUT}"
fi
compile "$@"
