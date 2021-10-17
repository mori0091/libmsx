# -*- coding: utf-8-unix -*-
#/bin/sh

bin2c() {
    printf 'const char %s[] = {\n' ${1}
    hexdump -v -e '"  " /1 "0x%02x, " "\n"'
    printf '};\n'
}

help() {
    cat - <<END
Converts binary file to C source.

Usage: bin2c.sh [options] file

Options:
  -h, --help     display this help and exits.
  -o FILE        specify output filename.
  -n NAME        specify name of the char array.

Example:
  The below makes 'foo.c' from a binary file 'foo.bin':
  ~~~shell
  bin2c.sh -n foo -o foo.c foo.bin
  ~~~

END
}

check_option_argument_required() {
    if [ "x${2}" == "x" ] ; then
        echo "an argument is required '$1'"
        exit 1
    fi
    case $2 in
        -*)
            echo "invalid argument for '$1'"
            exit 1
            ;;
    esac
}

infile=;
outfile=;
name=;

if [ $# -eq 0 ]; then
    help
    exit 0
fi

while [ $# -gt 0 ]; do
    case $1 in
        -h | --help)
            help
            exit 0
            ;;
        -o)
            check_option_argument_required "$@"
            outfile=$2
            shift
            ;;
        -n)
            check_option_argument_required "$@"
            name=$2
            shift
            ;;
        -*)
            echo "unknown option '$1'"
            exit 1
            ;;
        *)
            if [ "${infile}" ] ; then
                echo "too many files"
                exit 1
            fi
            infile=$1
            ;;
    esac
    shift
done

[ -f "${infile}" ] || {
    echo "file not found"
    exit 1
}

size=$(stat -c '%s' ${infile})
{
    echo "#include <stdlib.h>"
    echo
    echo "const size_t ${name}_size = ${size};"
    bin2c ${name}
} < ${infile} > ${outfile}
