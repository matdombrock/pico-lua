#!/bin/bash
source env.src
mkdir -p ./lua_h
./picolua_convert.sh tools ./lua_src/tools.lua ./lua_h/tools.lua.h
./picolua_convert.sh pico ./lua_src/pico.lua ./lua_h/pico.lua.h
./picolua_convert.sh default ./lua_src/default.lua ./lua_h/default.lua.h
./picolua_convert.sh system ./lua_src/system.lua ./lua_h/system.lua.h
# cp -f picolua.h ./test
mkdir build
rm -rf build/*
cd build
cmake .. -DPICO_SDK_PATH=$PICO_SDK_PATH -DPICO_BOARD=pico2
make -j4
