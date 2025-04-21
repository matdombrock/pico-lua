/*
** $Id: loslib.c $
** Standard Operating System library
** See Copyright Notice in lua.h
*/

/*
 * This is a VERY stripped down version of the original OS lib for embedded use
 * Mathieu Dombrock (Replicat) GPL3
*/

#define LUA_LIB

#include "lprefix.h"

/* For Pico RP2350, disable system-specific functions */
#define LUA_PICO_RP2350


#include <errno.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "lua.h"

#include "lauxlib.h"
#include "lualib.h"


// Time in seconds since boot
static int os_time (lua_State *L) {
  lua_pushnumber(L, ((float)clock())/(lua_Number)CLOCKS_PER_SEC);
  return 1;
}

// Raw clock value
// On RP2350 with pico-sdk, this is actually the number of ms since boot and NOT the number of ticks
static int os_clock (lua_State *L) {
  lua_pushinteger(L, clock());
  return 1;
}

// we have no env variables on RP2350
static int os_getenv (lua_State *L) {
  lua_pushnil(L);
  return 1;
}

// Always return C locale on RP2350
static int os_setlocale (lua_State *L) {
  lua_pushstring(L, "C");
  return 1;
}

// On RP2350, we don't actually exit
static int os_exit (lua_State *L) {
  if (lua_toboolean(L, 2))
    lua_close(L);
  return 0;
}


static const luaL_Reg syslib[] = {
  {"clock",     os_clock},
  {"time",      os_time},
  {"exit",      os_exit},
  {"getenv",    os_getenv},
  {"setlocale", os_setlocale},
  {NULL, NULL}
};

LUAMOD_API int luaopen_os (lua_State *L) {
  luaL_newlib(L, syslib);
  return 1;
}
