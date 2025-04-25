/**
 * Lua test for Raspberry Pi Pico RP2350
 */

#include "pico/stdlib.h"
#include "hardware/flash.h"
#include "hardware/sync.h" // For flash
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Include Lua headers for C
// #include "../../lib/lua/lua-5.4.6/include/lua.h"
// #include "../../lib/lua/lua-5.4.6/include/lauxlib.h"
// #include "../../lib/lua/lua-5.4.6/include/lualib.h"

// Use the .hpp file for C++
#include "../../lib/lua/lua-5.4.6/include/lua.hpp"

// System tools for pico end
#include "lua/pico.lua.h"
// Local the custom lua library file
#include "lua/tools.lua.h"
// Load our default main user script that was generated with the `convert_lua.sh` script
#include "lua/default.lua.h"
// Load the hander
#include "lua/system.lua.h"

// Right now we just use GPIO 0 for a test
#define DEFAULT_LED 0
                       
//
// UTILITY FUNCTIONS
//

// Returns the size of the flash memory used by the program
size_t pl_flash_info() {
  // Get the address of the end of the program
  extern uint8_t __flash_binary_end; // Provided by the linker
  size_t used_flash = (size_t)(&__flash_binary_end) - XIP_BASE;
  return used_flash;
}

// Initialize the GPIO for the LED
void pl_pico_gpio_init(int pin) {
  gpio_init(pin);
  gpio_set_dir(pin, GPIO_OUT);
}

// Turn the GPIO on or off
void pl_pico_gpio_set(int pin, bool state) {
  gpio_put(pin, state);
}

//
// LUA BINDS  
//

static int pl_lua_gpio_init(lua_State *L) {
  int pin = luaL_checkinteger(L, 1);
  pl_pico_gpio_init(pin);
  return 0;
}

// Lua function to control the GPIO
static int pl_lua_gpio_set(lua_State *L) {
  int pin = luaL_checkinteger(L, 1);
  int state = lua_toboolean(L, 2);
  pl_pico_gpio_set(pin, state);
  return 0;
}

// Lua function to sleep for milliseconds
// Sleeps on the main thread
// Prefer using `pico.wait()` defined in the lua code which is non-blocking
static int pl_lua_sleep_ms(lua_State *L) {
  int ms = luaL_checkinteger(L, 1);
  sleep_ms(ms);
  return 0;
}

// Returns the amount of flash used by the program
static int pl_lua_flash_used(lua_State *L) {
  size_t used = pl_flash_info();
  lua_pushinteger(L, used);
  return 1;
}

// Get the current time in microseconds at the start
// Uses the pico-sdk
static int pl_lua_clock(lua_State *L) {
  uint64_t time = time_us_64();
  lua_pushinteger(L, time);
  return 1;
}


// Register our Pico-specific functions to Lua
static const struct luaL_Reg pl_pico_lib[] = {
  {"gpio_init", pl_lua_gpio_init},
  {"gpio_set", pl_lua_gpio_set},
  {"sleep_ms", pl_lua_sleep_ms},
  {"flash_used", pl_lua_flash_used},
  {"clock", pl_lua_clock},
  {NULL, NULL}
};

// Initialize Lua and register Pico functions
lua_State *pl_init_lua(const struct luaL_Reg pl_app_lib[]) {
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);

  // Register our custom functions
  luaL_newlib(L, pl_pico_lib);
  lua_setglobal(L, "pico");

  luaL_newlib(L, pl_app_lib);
  lua_setglobal(L, "app");

  return L;
}

#define BUFFER_SIZE_SERIAL 128
char* pl_serial_read_ln() {
  char buffer[BUFFER_SIZE_SERIAL];
  int reading = 1;
  while (reading) { // TODO: IDK about this loop
    printf(">> ");
    // Clear the buffer
    memset(buffer, 0, BUFFER_SIZE_SERIAL);

    // Read a line of input (terminated by '\n')
    int index = 0;
    while (index < BUFFER_SIZE_SERIAL - 1) {
      char c = getchar(); // Read a character from UART
      printf("%c", c);
      if (c == '\n' || c == '\r') break;
      buffer[index++] = c;
    }

    // Null-terminate the string
    buffer[index] = '\0';
    reading = 0;
  }

  // Allocate memory for the resulting string
  char *result = (char *)malloc(strlen(buffer) + 1);
  if (result == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    exit(1);
  }
  strcpy(result, buffer);

  return result;
}

char* pl_serial_load_lua() {
  int reading = 1;
  // Initialize with an initial buffer size
  size_t buffer_size = 1024;  // Adjust size as needed
  char *lua_lines = (char *)malloc(buffer_size);
  if (!lua_lines) {
    // Handle memory allocation failure
    return NULL;
  }
  // Initialize the string buffer to empty
  lua_lines[0] = '\0';

  int line_num = 0;
  while (reading) {
    char *read = pl_serial_read_ln();
    // Check if we need more space
    if (strlen(lua_lines) + strlen(read) + 1 > buffer_size) {
      buffer_size *= 2;  // Double the buffer size
      char *temp = (char *)realloc(lua_lines, buffer_size);
      if (!temp) {
        // Handle memory allocation failure
        free(lua_lines);
        free(read);
        return NULL;
      }
      lua_lines = temp;
    }

    if (strcmp(read, "{{{END}}}") == 0) {
      reading = 0;
      free(read);
      continue;
    }

    strcat(lua_lines, read);
    strcat(lua_lines, "\n");  // Add newline after each line

    line_num++;

    printf("Lua #%d: %s\n", line_num, read);
    free(read);
  }

  printf("\nLua lines:\n%s", lua_lines);
  // Don't free lua_lines anymore since we're returning it
  return lua_lines;
}

char *pl_serial_load_lua_hander() {
  pl_pico_gpio_set(DEFAULT_LED, 1);
  printf("Starting Lua loading...\n");
  char *lua_script_serial = pl_serial_load_lua();
  printf("Lua loading complete.\n"); 
  // Check the size of the script
  size_t lua_script_serial_size = strlen(lua_script_serial);
  if (lua_script_serial_size > 0) {
    printf("Lua script size: %zu bytes\n", lua_script_serial_size);
  } else { /// Sending an empty script returns NULL
    printf("Lua script is empty or failed to load.\n");
    lua_script_serial = NULL;
  }
  // Blink to confirm
  for (int i = 0; i < 8; i++) {
    pl_pico_gpio_set(DEFAULT_LED, 0);
    sleep_ms(80);
    pl_pico_gpio_set(DEFAULT_LED, 1);
    sleep_ms(80);
  }
  return lua_script_serial;
}

int pl_init(const struct luaL_Reg app_lib[], int load_serial) {
  // Initialize standard I/O
  stdio_init_all();

  // Initialize the LED
  pl_pico_gpio_init(DEFAULT_LED);

  // Ensure we can see the output
  sleep_ms(2000);

  char *lua_script_serial = NULL;
  if (load_serial) lua_script_serial = pl_serial_load_lua_hander();
  printf("\n\n=== Starting Lua on Pico RP2350 ===\n\n");

  // Initialize Lua
  lua_State *L = pl_init_lua(app_lib);
  if (!L) {
    printf("Failed to initialize Lua\n");
    return 1;
  }

  int result_pico = luaL_dostring(L, pl_script_pico);
  if (result_pico != LUA_OK) {
    printf("Lua pico error: %s\n", lua_tostring(L, -1));
    lua_pop(L, 1); // Remove traceback from stack
  }
  else {
    printf("Loaded lua pico extensions ok\n");
  }
  int result_tools = luaL_dostring(L, pl_script_tools);
  if (result_tools != LUA_OK) {
    printf("Lua tools error: %s\n", lua_tostring(L, -1));
    lua_pop(L, 1); // Remove traceback from stack
  }
  else {
    printf("Loaded lua tools ok\n");
  }
  const char *user_script_target = lua_script_serial != NULL ? lua_script_serial : pl_script_default;
  int result_script = luaL_dostring(L, user_script_target);
  if (result_script != LUA_OK) {
    printf("Lua user script error: %s\n", lua_tostring(L, -1));
    lua_pop(L, 1); // Remove traceback from stack
  }
  else {
    printf("Loaded lua user script ok\n");
  }
  int result_system = luaL_dostring(L, pl_script_system);
  if (result_system != LUA_OK) {
    printf("Lua system error: %s\n", lua_tostring(L, -1));
    lua_pop(L, 1); // Remove traceback from stack
  }
  else {
    printf("Loaded lua system ok\n");
  }
  // Clean up Lua
  lua_close(L);

  printf("\n=== Lua script execution complete ===\n");
  printf("Entering normal LED blink loop...\n"); 
}
