/**
 * Lua test for Raspberry Pi Pico RP2350
 */

#include "pico/stdlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include Lua headers
#include "../lib/lua/lua-5.4.6/include/lua.h"
#include "../lib/lua/lua-5.4.6/include/lauxlib.h"
#include "../lib/lua/lua-5.4.6/include/lualib.h"

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

static int lua_check_flash(lua_State *L) {
    size_t storage = check_flash_storage(0);
    lua_pushinteger(L, storage);
    size_t used = check_flash_storage(1);
    lua_pushinteger(L, used);
    size_t free = check_flash_storage(2);
    lua_pushinteger(L, free);
    return 3;
}


// Register our Pico-specific functions to Lua
static const struct luaL_Reg pico_lib[] = {
    {"led", lua_set_led},
    {"sleep_ms", lua_sleep_ms},
    {"flash_info", lua_check_flash},
    {NULL, NULL}
};

// Initialize Lua and register Pico functions
lua_State *init_lua(void) {
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    
    // Register our custom functions
    luaL_newlib(L, pico_lib);
    lua_setglobal(L, "pico");
    
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
    lua_State *L = init_lua();
    if (!L) {
        printf("Failed to initialize Lua\n");
        return 1;
    }
    

    // Combine the helper lib and the lua script
    // char *comb_script = malloc(strlen(lua_tools) + strlen(lua_script) + strlen(lua_handler) + 1);
    // strcpy(comb_script, lua_tools);
    // strcat(comb_script, "\n");
    // strcat(comb_script, lua_script);  
    // strcat(comb_script, "\n");
    // // Append the lua_hander script
    // strcat(comb_script, lua_handler);
    // // Run the Lua script
    // printf("Executing Lua script:\n%s\n=== end of script ===\n", comb_script);
    // int result = luaL_dostring(L, comb_script);
    // free(comb_script); 
    // if (result) {
    //     printf("Lua user script error: %s\n", lua_tostring(L, -1));
    // }
    
    int result_tools = luaL_dostring(L, lua_tools);
    if (result_tools) {
        printf("Lua tools error: %s\n", lua_tostring(L, -1));
    }
    int result = luaL_dostring(L, lua_script);
    if (result) {
        printf("Lua user script error: %s\n", lua_tostring(L, -1));
    }
    int result_handler = luaL_dostring(L, lua_handler);
    if (result_handler) {
      printf("Lua handler error: %s\n", lua_tostring(L, -1));
    }
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
