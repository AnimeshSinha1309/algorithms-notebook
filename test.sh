#!/bin/bash

# Copy from https://online-judge-tools.readthedocs.io/en/master/run-ci-on-your-library.html

# Copyright (c) 2017-2018 Kimiyuki Onaka.
# The MIT License (MIT): https://github.com/kmyk/online-judge-tools/blob/master/LICENSE

set -e
which oj > /dev/null

CXX=${CXX:-g++}
CXXFLAGS="${CXXFLAGS:--std=c++14 -fsanitize=address -fsanitize=undefined -O2 -Wall -g}"
ulimit -s unlimited

run() {
    file="$1"
    url="$(grep -o '^\/\/ PROBLEM: \+\(https\?://.*\)' < "$file" | sed 's/.* http/http/')"
    dir=.scratch/$(echo -n "$url" | md5sum | sed 's/ .*//')
    mkdir -p ${dir}
    $CXX $CXXFLAGS -I . -o ${dir}/a.out "$file"
    if [[ -n ${url} ]] ; then
        if [[ ! -e ${dir}/test ]] ; then
            sleep 2
            oj download --system "$url" -d ${dir}/test
        fi
        oj test --tle 10 --c ${dir}/a.out -d ${dir}/test
    else
        ${dir}/a.out
    fi
}

if [ $# -eq 0 ] ; then
    for f in $(find . -name \*.test.cpp) ; do
        run $f
    done
else
    for f in "$@" ; do
        run "$f"
    done
fi