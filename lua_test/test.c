/**
 * Lua test for Raspberry Pi Pico RP2350
 */

#include "pico/stdlib.h"
#include <stdio.h>

// Include Lua headers
#include "../lib/lua/lua-5.4.6/include/lua.h"
#include "../lib/lua/lua-5.4.6/include/lauxlib.h"
#include "../lib/lua/lua-5.4.6/include/lualib.h"

#ifndef LED_DELAY_MS
#define LED_DELAY_MS 250
#endif

#ifndef PICO_DEFAULT_LED_PIN
#warning blink_simple example requires a board with a regular LED
#endif

// Initialize the GPIO for the LED
void pico_led_init(void) {
#ifdef PICO_DEFAULT_LED_PIN
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
#endif
}

// Turn the LED on or off
void pico_set_led(bool led_on) {
#if defined(PICO_DEFAULT_LED_PIN)
    gpio_put(PICO_DEFAULT_LED_PIN, led_on);
#endif
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

// Simple Lua script to blink the LED
const char *blink_script = 
    "print('Lua running on Pico RP2350!')\n"
    "for i = 1, 10 do\n"
    "  print('this is LUA!')"
    "  print('LED on, cycle ' .. i)\n"
    "  pico.led(true)\n"
    "  pico.sleep_ms(500)\n"
    "  print('LED off, cycle ' .. i)\n"
    "  pico.led(false)\n"
    "  pico.sleep_ms(500)\n"
    "end\n"
    "print('Script completed!')\n";

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
    
    printf("Executing Lua script:\n%s\n", blink_script);
    
    // Run the Lua script
    int result = luaL_dostring(L, blink_script);
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
