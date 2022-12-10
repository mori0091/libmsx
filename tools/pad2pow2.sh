#!/bin/sh
# -*- coding: utf-8-unix -*-

while [ $# -gt 0 ] ; do
    if [ -f "$1" ] ; then
        size="$(stat -c '%s' "$1")";
        x=16384;
        while [ "${x}" -lt "${size}" ] ; do
            x="$(( x * 2 ))";
        done
        truncate -s "${x}" "$1"
    fi
    shift
done
