#!/bin/bash
set -e
oj --version

CXX=${CXX:-g++}
CXXFLAGS="${CXXFLAGS:--std=c++14 -O2 -Wall -g}"
ulimit -s unlimited  # make the stack size unlimited

# list files to test
for file in $(find . -name \*.test.cpp) ; do

    # get the URL for verification
    url="$(sed -e 's/^# *define \+PROBLEM \+"\(https\?:\/\/.*\)"/\1/ ; t ; d' "$file")"
    if [[ -z ${url} ]] ; then
        continue
    fi

    dir=cache/$(echo -n "$url" | md5sum | sed 's/ .*//')
    mkdir -p ${dir}

    # download sample cases
    if [[ ! -e ${dir}/test ]] ; then
        sleep 2
        oj download --system "$url" -d ${dir}/test
    fi

    # run test
    $CXX $CXXFLAGS -I . -o ${dir}/a.out "$file"
    oj test --tle 10 --c ${dir}/a.out -d ${dir}/test
done
