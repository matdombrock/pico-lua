# PICOLUA

A library that allows for running a Lua interpreter in a C/C++ program targeting the pico-sdk for RP2350.

This is a way to run a Lua *interpreter* from within your C/C++ program.

This is not a way to write embedded applications fully in Lua (see [MicroLua](https://github.com/MicroLua/MicroLua) for that).

## Build Patched Lua Version

```bash
./picolua_patch.sh
```

The patched Lua files will be in `./lib/lua/`

Copy the Lua files to your project as needed:

```bash
cp -r ./lib/lua/ ../my_project/lib/lua
```

## Add `picolua` to your project

```bash
cp -r ./picolua/ ../my_project/src
```

The `./picolua` directory contains the `picolua` library and the `picolua.h` header file as well as the Lua files which provide interpreter functions.

## Modify your `CmakeLists.txt`

Add the following to your `CMakeLists.txt` file. Be sure to replace `<your_project>` with the name of your project and replace the Lua library path with the correct path to your Lua library.

```CmakeLists
# CMakeLists.txt

# Set path to Lua library and include directories
set(LUA_DIR "${CMAKE_SOURCE_DIR}/../../lib/lua/lua-5.4.6/")
set(LUA_INCLUDE_DIR "${LUA_DIR}/include")
set(LUA_LIBRARY_DIR "${LUA_DIR}")

# Include directories for lua headers
target_include_directories(<your_project> PRIVATE
    ${LUA_INCLUDE_DIR}
)

# Link against the static Lua library
target_link_directories(<your_project> PRIVATE
    ${LUA_LIBRARY_DIR}
)

# Pull in dependencies
target_link_libraries(<your project> 
    pico_stdlib
    liblua.a
)
```

Note: You will also need to run the `./picolua/picolua_convert.sh` script before build to convert the Lua files to C header files.

## The C/C++ Code

### Include the `picolua` header

```c
// main.c
#include "picolua/picolua.h"
```

You do not need to include the Lua headers in your code. The `picolua` header will include them for you.

### Write some C code to be called by Lua

```c
// main.c
int my_state_val = 1;

// Register a new function to be used with Lua
static int lua_set_state(lua_State *L) {
  // Get a value from the Lua stack
  int new_state = luaL_checkinteger(L, 1);
  // Modify the state value
  new_state *= 2;
  // Push it back to the global C value
  my_state_val = new_state;
  // Push the new value to the Lua stack
  lua_pushinteger(L, my_state_val);
  // We are returning 1 value to Lua
  return 1;
}
```

### Add functions to run before and after the main loop

```c
// main.c
// This function will be called before the main loop
// You can add any init or cleanup code here
// This is optional
static int lua_before_main_loop(lua_State *L) { 
  int tick = luaL_checkinteger(L, 1);
  printf("Lua before main loop (tick: %d)\n", tick);
  return 0;
}

// This function will be called after the main loop
// You can add any cleanup or finalization code here
// This is optional
static int lua_after_main_loop(lua_State *L) { 
  int tick = luaL_checkinteger(L, 1);
  int runtime = luaL_checkinteger(L, 2);
  printf("Lua after main loop (tick: %d)\n", tick);
  printf("Runtime: %d microseconds\n", runtime);
  return 0;
}
```

### Run the Lua interpreter

```c
// main.c
int main() {
  // Set up some custom lua bindings for our app
  // These will be available in lua under `app.*`
  static const struct luaL_Reg app_lib[] = {
    {"set_state", lua_set_state}, 
    {"before_main_loop", lua_before_main_loop},
    {"after_main_loop", lua_after_main_loop},
    {NULL, NULL}
  };
  // Init the lua binding system and run the lua scripts
  pl_init(app_lib, true); // Load from serial 

  // Continue with regular C code when done with the Lua
  printf("Continuing with C code...\n");
  while (true) {
    pl_pico_gpio_set(0, true);
    sleep_ms(200);
    pl_pico_gpio_set(0, false);
    uint64_t stime = time_us_64();
    sleep_ms(200);
    uint64_t ttime = stime - time_us_64();
    printf("C code running, time: %d\n", ttime);
  }
}
```

## The Lua Code

There are 4 files which are part of `picolua` written in Lua itself.

- `default.lua`
- `pico.lua`
- `system.lua`
- `tools.lua`

### `default.lua`

This file holds the default user script which will be loaded and run when the system is unable to load any other script.

This should be modified to suit the needs of your application.

### `pico.lua`

This file provides low level API bindings to the Pico SDK.

This is part of the core `picolua` library and usually should not be modified.

### `system.lua`

This file contains the required system functions to run the Lua interpreter. It handles calling `Main_loop` etc.

This is part of the core `picolua` library and usually should not be modified.

### `tools.lua`

This file provides an application specific library to the end user (lua author).

This allows application developers to offer custom Lua functions to the end user which are written in Lua as opposed to C/C++.

For example an application which is very math heavy might provide a `app.abs_sin(x)` function which is a wrapper around Lua's `math.sin(x)` function providing an absolute sine value.

While these functions could be written in C/C++ and bound to Lua, it is often easier to write them in Lua itself.

This should obviously be modified to suit the needs of your application.

## User Scripting

User scripts must always define a `Main_loop(tick, delta)` function which will be called by the `picolua` library.

An example user script might look something like this:

```lua
function Main_loop(tick, delta)
  -- This function will be called every tick
  -- You can use the tick and delta values to do your work
  -- The tick value is the current tick count
  -- The delta value is the time since the last tick in microseconds
  print("Main loop tick: " .. tick .. ", delta: " .. delta)
end
```

It is possible to define variables and functions outside of the `Main_loop` function. These will be available in the global scope.

```lua

local x = 0

local function add(a, b)
  return a + b
end

function Main_loop(tick, delta)
  local y  = 2
  x = add(x, y)
  print("Main loop tick: " .. x)
end
```

## Lua Life Cycle

The `Main_loop` function will be called every tick and the `tick` and `delta` values will be passed to it.

The life cycle is as follows:

1) Initialize C code
2) Initialize Lua interpreter
3) Picolua calls the `lua_before_main_loop(tick)` C function
4) Picolua calls the `Main_loop(tick, delta)` Lua function
5) Picolua calls the `lua_after_main_loop(tick, runtime)` C function
6) Go to step 3

## Loading Scripts via Serial

When initializing the `picolua` library in your C code, you can set the second argument to `true` to load the scripts from the serial port.

```c
// main.c
// Init the lua binding system and run the lua scripts
pl_init(app_lib, true); // Load from serial 
```

If you have an LED hooked up to GPIO 0 it will light up to confirm the device is waiting for serial input.

You can now send your Lua scripts to the device over serial. Use `minicom` or see the `send_serial.sh` script in the example project for an example of how to do this.

When you have finished sending your script. Send a new line with the text:

```
{{{END}}}\n
```

The LED will blink to acknowledge the end of the script and the script will be loaded and run.
