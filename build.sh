#!/bin/bash
source env.src
cd ./lua_test
mkdir build
rm -rf build/*
cd build
cmake .. -DPICO_SDK_PATH=$PICO_SDK_PATH -DPICO_BOARD=pico2
make -j4
