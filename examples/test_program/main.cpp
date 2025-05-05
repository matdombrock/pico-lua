#include "pico/stdlib.h"
#include "picolua/picolua.h"

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
