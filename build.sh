#!/bin/bash
source env.src
./convert_lua.sh tools ./lua_src/tools.lua ./test_program/picolua/lua/tools.lua.h
./convert_lua.sh pico ./lua_src/pico.lua ./test_program/picolua/lua/pico.lua.h
./convert_lua.sh default ./lua_src/default.lua ./test_program/picolua/lua/default.lua.h
./convert_lua.sh system ./lua_src/system.lua ./test_program/picolua/lua/system.lua.h
# cp -f picolua.h ./test
cd ./test_program
mkdir build
rm -rf build/*
cd build
cmake .. -DPICO_SDK_PATH=$PICO_SDK_PATH -DPICO_BOARD=pico2
make -j4
