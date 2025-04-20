# PICO LUA
Run lua on a pico (2)

## Use
```bash
# Install deps (Fedora only)
# If you have built pico software before, you can likely skip this step
./deps.sh

# Download and modify the Lua source code
# This can not be skipped
./get_lua.sh

# Get the Pico SDK (needed even if you have it)
# This code uses a relative path for the PICO_SDK easiest to just let a copy live here
# Removing refs to `env.src` and providing your own $PICO_SDK_PATH env var can override this
./get_pico_sdk.sh

# Get and install the picotool binary 
# This is NOT needed if you have it installed and in your path)
./get_picotool.sh

# Build the example code in `lua_test`
./build.sh

# Flash the example code
./flash.sh lua_test/build/test.uf2 # Or however you normally flash
```
```

## Scripting
Edit the script at `./script.lua`. 

This will be automatically built and included into the compiled C code. (see `convert_lua.sh`)

Right now this simple API has 2 methods:
`pico.led(enabled)` - Enable or disable the led
`pico.sleep_ms` - Sleep for a number of milliseconds

