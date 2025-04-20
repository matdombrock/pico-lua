# PICO LUA
Run lua on a pico (2)

# Setup
```
```bash
# Install deps (Fedora only)
./deps.sh
# Download and mofify the Lua source code
./get_lua.sh
# Get the Pico SDK (needed even if you have it)
./get_pico_sdk.sh
# Get and install the picotool binary (not needed if you have it installed and in your path)
./get_picotool.sh
# Build the example code in `lua_test`
./build.sh
# Flsh the example code
./flash.sh lua_test/build/test.uf2
```
```
