#!/bin/bash
source env.src
./convert_lua.sh tools ./lua_src/tools.lua ./test_program/lua/tools.lua.h
./convert_lua.sh script ./lua_src/script.lua ./test_program/lua/script.lua.h
./convert_lua.sh handler ./lua_src/handler.lua ./test_program/lua/handler.lua.h
cd ./test_program
mkdir build
rm -rf build/*
cd build
cmake .. -DPICO_SDK_PATH=$PICO_SDK_PATH -DPICO_BOARD=pico2
make -j4
