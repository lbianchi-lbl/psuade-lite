#!/bin/bash

set -x -e

SOURCE_PATH="$SRC_DIR"
BIN_PATH="$PREFIX/bin"

mkdir -pv "$BIN_PATH"

cmake "$SOURCE_PATH" -DCMAKE_CXX_FLAGS="--std=c++14" -DCMAKE_INSTALL_PREFIX="$PREFIX" -DCMAKE_OSX_SYSROOT="$CONDA_BUILD_SYSROOT"
cmake --build . --target install
# cp ./bin/* "$BIN_PATH"