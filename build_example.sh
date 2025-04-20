#!/bin/bash
source env.src
cd ./lib/pico-examples
mkdir build
rm -rf build/*
cd build
cmake .. -DPICO_SDK_PATH=$PICO_SDK_PATH -DPICO_BOARD=pico2
make -j4 blink
