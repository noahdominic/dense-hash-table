#!/bin/sh

mkdir -p build
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -B build/ && make --directory=build/ && ./build/out "$1"
