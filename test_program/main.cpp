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
int main() {
  // Set up some custom lua bindsings for our app
  // These will be available in lua under `app.*`
  static const struct luaL_Reg app_lib[] = {
    {"set_state", lua_set_state}, 
    {NULL, NULL}
  };
  // Init the lua binding system and run the lua scripts
  pl_init(app_lib); 

  // Continue with regular C code when done with the Lua
  while (true) {
    pl_pico_set_led(true);
    sleep_ms(200);
    pl_pico_set_led(false);
    sleep_ms(200);
  }
}
