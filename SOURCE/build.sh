#!/usr/bin/env bash

if [ -z "$1" ]; then
    BUILD_TYPE="debug"
else
    BUILD_TYPE="$1"
fi

if [[ "$1" == r* ]]; then
    BUILD_TYPE="release"
fi
BUILD_DIR="build/${BUILD_TYPE}"
mkdir -p "$BUILD_DIR"

cmake -S . -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
cmake --build "$BUILD_DIR" -j$(nproc)
