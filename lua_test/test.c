/**
 * Lua test for Raspberry Pi Pico RP2350
 */

#include "pico/stdlib.h"
#include <stdio.h>

// Include Lua headers
#include "../lib/lua/lua-5.4.6/include/lua.h"
#include "../lib/lua/lua-5.4.6/include/lauxlib.h"
#include "../lib/lua/lua-5.4.6/include/lualib.h"

// Load our script that was generated with the `convert_lua.sh` script
#include "script.lua.h"

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

// Register our Pico-specific functions to Lua
static const struct luaL_Reg pico_lib[] = {
    {"led", lua_set_led},
    {"sleep_ms", lua_sleep_ms},
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
    
    printf("Executing Lua script:\n%s\n", lua_script);
    
    // Run the Lua script
    int result = luaL_dostring(L, lua_script);
    if (result) {
        printf("Lua error: %s\n", lua_tostring(L, -1));
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
