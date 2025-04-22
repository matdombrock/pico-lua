/**
* Lua test for Raspberry Pi Pico RP2350
*/

#include "pico/stdlib.h"
#include "hardware/flash.h"
#include "hardware/sync.h" // For flash
#include <stdio.h>
#include <stdlib.h>

// Include Lua headers for C
// #include "../lib/lua/lua-5.4.6/include/lua.h"
// #include "../lib/lua/lua-5.4.6/include/lauxlib.h"
// #include "../lib/lua/lua-5.4.6/include/lualib.h"

// Use the .hpp file for C++
#include "../lib/lua/lua-5.4.6/include/lua.hpp"

// Local the custom lua library file
#include "lua/tools.lua.h"
// Load our main user script that was generated with the `convert_lua.sh` script
#include "lua/script.lua.h"
// Load the hander
#include "lua/handler.lua.h"

// Right now we just use GPIO 0 for a test
#define LED_PIN 0
// Define the size of the flash memory
#define FLASH_SIZE_BYTES (4 * 1024 * 1024) // 4MB of flash memory by default on pico2 dev boards
// Define the start of the program in flash memory
#define FLASH_BASE_ADDR XIP_BASE

// (total_flash | used_flash | free_flash)
size_t check_flash_storage(int mode) {
    // Calculate total flash size
    size_t total_flash = FLASH_SIZE_BYTES;

    // Get the address of the end of the program
    extern uint8_t __flash_binary_end; // Provided by the linker
    size_t used_flash = (size_t)(&__flash_binary_end) - FLASH_BASE_ADDR;

    // Calculate free flash memory
    size_t free_flash = total_flash - used_flash;

    switch (mode) {
      case 0:
        return total_flash;
      case 1:
        return used_flash;
      case 2:
        return free_flash;
      default:
        printf("Invalid mode. Use 0 for total, 1 for used, or 2 for free.\n");
        return 0;
    }
}

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
static int lua_set_led(lua_State *L) {
 int on = lua_toboolean(L, 1);
 pico_set_led(on);
 return 0;
}

// Lua function to sleep for milliseconds
static int lua_sleep_ms(lua_State *L) {
 int ms = luaL_checkinteger(L, 1);
 sleep_ms(ms);
 return 0;
}

// Lua function to check the flash stats
// Returns used, free, total
static int lua_check_flash(lua_State *L) { 
 size_t used = check_flash_storage(1);
 lua_pushinteger(L, used);
 size_t free = check_flash_storage(2);
 lua_pushinteger(L, free);
 size_t total = check_flash_storage(0);
 lua_pushinteger(L, total);
 return 3;
}

// Get the current time in microseconds at the start
static int lua_clock(lua_State *L) {
 uint64_t time = time_us_64();
 lua_pushinteger(L, time);
 return 1;
}


// Register our Pico-specific functions to Lua
static const struct luaL_Reg pico_lib[] = {
 {"led", lua_set_led},
 {"sleep_ms", lua_sleep_ms},
 {"flash_info", lua_check_flash},
 {"clock", lua_clock},
 {NULL, NULL}
};

// Initialize Lua and register Pico functions
lua_State *init_lua(const struct luaL_Reg app_lib[]) {
 lua_State *L = luaL_newstate();
 luaL_openlibs(L);
   
 // Register our custom functions
 luaL_newlib(L, pico_lib);
 lua_setglobal(L, "pico");

 luaL_newlib(L, app_lib);
 lua_setglobal(L, "app");
   
 return L;
}

int picolua_init(const struct luaL_Reg app_lib[]) {
 // Initialize standard I/O
 stdio_init_all();
 
 // Initialize the LED
 pico_led_init();
 
 // Ensure we can see the output
 sleep_ms(2000);
 printf("\n\n=== Starting Lua on Pico RP2350 ===\n\n");
 
 // Initialize Lua
 lua_State *L = init_lua(app_lib);
 if (!L) {
     printf("Failed to initialize Lua\n");
     return 1;
 }
 

 int result_tools = luaL_dostring(L, lua_tools);
 if (result_tools) {
     luaL_traceback(L, L, lua_tostring(L, -1), 1);
     printf("Lua tools error: %s\n", lua_tostring(L, -1));
     lua_pop(L, 1); // Remove traceback from stack
 }
 int result_script = luaL_dostring(L, lua_script);
 if (result_script) {
     luaL_traceback(L, L, lua_tostring(L, -1), 1);
     printf("Lua user script error: %s\n", lua_tostring(L, -1));
     lua_pop(L, 1); // Remove traceback from stack
 }
 int result_handler = luaL_dostring(L, lua_handler);
 if (result_handler) {
     luaL_traceback(L, L, lua_tostring(L, -1), 1);
     printf("Lua handler error: %s\n", lua_tostring(L, -1));
     lua_pop(L, 1); // Remove traceback from stack
 }

 // Clean up Lua
 lua_close(L);
 
 printf("\n=== Lua script execution complete ===\n");
 printf("Entering normal LED blink loop...\n"); 
}
