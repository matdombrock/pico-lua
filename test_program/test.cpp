/**
 * Lua test for Raspberry Pi Pico RP2350
 */

#include "pico/stdlib.h"
#include <stdio.h>
#include <stdlib.h>

#include "../lib/lua/lua-5.4.6/src/lua.hpp"

// A tool for reading info about flash storage
#include "flash.h"

// Local the custom lua library file
#include "lua/tools.lua.h"
// Load our sscanfript that was generated with the `convert_lua.sh` script
#include "lua/script.lua.h"
// Load the hander
#include "lua/handler.lua.h"

#ifndef LED_DELAY_MS
#define LED_DELAY_MS 250
#endif

#define LED_PIN 0

// Initialize the GPIO for the LED
void pico_led_init(void) {
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);
}

// Turn the LED on or off
void pico_set_led(bool led_on) {
  gpio_put(LED_PIN, led_on);
}

// Lua function to control the LED
int lua_set_led(lua_State *L) {
  int on = lua_toboolean(L, 1);
  pico_set_led(on);
  return 0;
}

// Lua function to sleep for milliseconds
int lua_sleep_ms(lua_State *L) {
  int ms = (int)luaL_checknumber(L, 1);
  sleep_ms(1000);
  return 0;
}

// Lua function to check the flash stats
// Returns used, free, total
int lua_check_flash(lua_State *L) { 
  size_t used = check_flash_storage(1);
  lua_pushnumber(L, (lua_Number)used);
  size_t free = check_flash_storage(2);
  lua_pushnumber(L, (lua_Number)free);
  size_t total = check_flash_storage(0);
  lua_pushnumber(L, (lua_Number)total);
  return 3;
}

// Get the current time in microseconds at the start
int lua_clock(lua_State *L) {
  uint64_t time = time_us_64();
  lua_pushnumber(L, (lua_Number)time);
  return 1;
}

// Initialize Lua and register Pico functions
lua_State *init_lua() {
  // Register our Pico-specific functions to Lua
  // static const luaL_Reg pico_lib[] = {
  //   {"led", lua_set_led},
  //   {"sleep_ms", lua_sleep_ms},
  //   {"flash_info", lua_check_flash},
  //   {"clock", lua_clock},
  //   {nullptr, nullptr}
  // };
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);
    
  // Register our custom functions
  // luaL_newlib(L, pico_lib);
  lua_register(L, "pico_led", lua_set_led);
  lua_register(L, "pico_sleep_ms", lua_sleep_ms);
  lua_register(L, "pico_flash_info", lua_check_flash);
  lua_register(L, "pico_clock", lua_clock);
  

    // lua_getglobal(L, "pico");
    // if (lua_isnil(L, -1)) {
    //     lua_newtable(L); // Create a new table if 'pico' doesn't exist
    //     lua_setglobal(L, "pico"); // Set it as a global variable
    //     lua_getglobal(L, "pico"); // Push it back to the stack
    // }
    //
    // // Register 'flash' function in the 'pico' table
    // lua_pushcfunction(L, lua_check_flash);
    // lua_setfield(L, -2, "flash_info");
    // // Register the led function in the 'pico' table
    // lua_pushcfunction(L, lua_set_led);
    // lua_setfield(L, -2, "led");
    // lua_pushcfunction(L, lua_sleep_ms);
    // lua_setfield(L, -2, "sleep_ms");
    // lua_pushcfunction(L, lua_clock);
    // lua_setfield(L, -2, "clock");
    //
    // // Pop the 'pico' table from the stack
    // lua_pop(L, 1);

  // luaL_checkversion(L); // Ensure Lua version compatibility
  // lua_createtable(L, 0, sizeof(pico_lib) / sizeof(pico_lib[0]) - 1); // Create a new table
  // luaL_setfuncs(L, pico_lib, 0); // Register functions from pico_lib
  // lua_setglobal(L, "pico");
    
  return L;
}

int main() {
  // Initialize standard I/O
  stdio_init_all();
  
  // Initialize the LED
  pico_led_init();
  
  // Ensure we can see the output
  sleep_ms(2000);
  printf("\n\n=== Starting Lua on Pico RP2350 ===\n\n");
  
  // Initialize Lua
  lua_State* L = init_lua();
  // lua_State* L = luaL_newstate();
  // luaL_openlibs(L);
  // lua_close(L);
  // lua_State *L = init_lua();
  // if (!L) {
  //     printf("Failed to initialize Lua\n");
  //     return 1;
  // }
  

  int result_tools = luaL_dostring(L, lua_tools);
  // if (result_tools) {
  //     luaL_traceback(L, L, lua_tostring(L, -1), 1);
  //     printf("Lua tools error: %s\n", lua_tostring(L, -1));
  //     lua_pop(L, 1); // Remove traceback from stack
  // }
  int result_script = luaL_dostring(L, lua_script);
  // if (result_script) {
  //     luaL_traceback(L, L, lua_tostring(L, -1), 1);
  //     printf("Lua user script error: %s\n", lua_tostring(L, -1));
  //     lua_pop(L, 1); // Remove traceback from stack
  // }
  int result_handler = luaL_dostring(L, lua_handler);
  // if (result_handler) {
  //     luaL_traceback(L, L, lua_tostring(L, -1), 1);
  //     printf("Lua handler error: %s\n", lua_tostring(L, -1));
  //     lua_pop(L, 1); // Remove traceback from stack
  // }

  // Clean up Lua
  lua_close(L);
  
  printf("\n=== Lua script execution complete ===\n");
  printf("Entering normal LED blink loop...\n");
  
  // Continue with regular LED blinking pattern
  while (true) {
    pico_set_led(true);
    sleep_ms(LED_DELAY_MS);
    pico_set_led(false);
    sleep_ms(LED_DELAY_MS);
  }
}
