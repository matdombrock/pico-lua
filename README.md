# PICOLUA

A library that allows for running a Lua interpreter in a C program targeting the pico-sdk (RP2350). 

This is a way to run a Lua *interpreter* from within your C program. 

This is not a way to write embedded applications fully in Lua (see [MicroLua](https://github.com/MicroLua/MicroLua) for that).

## Lua Patching

This works by downloading the Lua source code and modifying it to work with the Pico SDK.

The main file being replaced is `loslib.c` which contains the Lua OS library.

See `patch_lua.sh` and `./lua_patches` for more info.

The patched Lua build can be found in `./lib/lua` after running `./patch_lua.sh`.

## Setup

Before you can build the example code, you need to run a few setup scripts.
```bash
# Install deps (Fedora only)
# If you have built pico software before, you can likely skip this step
./deps.sh

# Download and modify the Lua source code
# This can not be skipped
./patch_lua.sh

# Get the Pico SDK (needed even if you have it)
# This code uses a relative path for the PICO_SDK easiest to just let a copy live here
# Removing refs to `env.src` and providing your own $PICO_SDK_PATH env var can override this
./get_pico_sdk.sh

# Get and install the picotool binary 
# This is NOT needed if you have it installed and in your path)
./get_picotool.sh
```

## Building the example code

```bash
# Build the example code 
./build.sh

# Flash the example code
./flash.sh # Or however you normally flash
```


