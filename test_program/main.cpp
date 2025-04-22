#include "pico/stdlib.h"
#include "picolua.h"

// Register a new function to be used with Lua
static int lua_test(lua_State *L) {
  // Return funny number
  lua_pushinteger(L, 42069);
  return 1;
}
int main() {
  // Set up some custom lua bindsings for our app
  // These will be available in lua under `app.*`
  static const struct luaL_Reg app_lib[] = {
    {"test", lua_test}, 
    {NULL, NULL}
  };
  // Init the lua binding system and run the lua scripts
  picolua_init(app_lib); 

  // Continue with regular C code when done with the Lua
  while (true) {
    pico_set_led(true);
    sleep_ms(200);
    pico_set_led(false);
    sleep_ms(200);
  }
}
