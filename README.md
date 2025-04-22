# PICO LUA

Run a Lua interpreter on an RP2350 Pico board.

This is a way to run a Lua *interpreter* not a way to write entire embedded applications in Lua (see [MicroLua](https://github.com/MicroLua/MicroLua) for that).

The core application logic is still written in C / C++.

## How this works

This works by downloading the Lua source code and modifying it to work with the Pico SDK.

The main file being replaced is loslib.c which is the Lua OS library.

See `patch_lua.sh` and `./lua_patches` for more info.

## Use

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

# Build the example code 
./build.sh

# Flash the example code
./flash.sh # Or however you normally flash
```

## Scripting

Edit the script at `./lua_src_usr/script.lua`.

This will be automatically build and included into the compiled C code. (see `convert_lua.sh`)

The Lua code in `./lua_src_sys/tools.lua` will be provided to all user scripts under the `tools` object.

The main `./lua_src_usr/script.lua` should define a `Main_loop(ticks)` function. This will be called periodically by the Lua code in `./lua_src_sys/handler.lua` and automatically supports async operations using Lua coroutines.

See the existing script for examples of Lua API usage.
