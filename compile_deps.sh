#!/bin/bash

cd 3rdparty || exit
THIRD_PARTY_ROOT=$(pwd)

cd "$THIRD_PARTY_ROOT" || exit
tar xpf blend2d-beta9-all.tar.xz
cd blend2d || exit
mkdir build
cd build || exit
cmake .. -DBLEND2D_BUILD_STATIC=ON -DCMAKE_INSTALL_PREFIX="${THIRD_PARTY_ROOT}"
make -j4 && make install

cd "$THIRD_PARTY_ROOT" || exit
tar xfp SDL2-2.0.8.tar.gz
cd SDL2-2.0.8 || exit
mkdir build
cd build || exit
cmake .. -DCMAKE_INSTALL_PREFIX="${THIRD_PARTY_ROOT}" -DSDL_SHARED=OFF -DSDL_STATIC=ON
make -j 4 && make install