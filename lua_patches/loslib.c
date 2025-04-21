/*
** $Id: loslib.c $
** Standard Operating System library
** See Copyright Notice in lua.h
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


/*
** {==================================================================
** Time/Date operations
** ===================================================================
*/

/*
** Time in seconds since start; Pico-specific implementation
*/
static int os_clock (lua_State *L) {
  lua_pushnumber(L, ((lua_Number)clock())/(lua_Number)CLOCKS_PER_SEC);
  return 1;
}


/*
** Time in seconds since the Unix epoch (1970). 
** This implementation is simplified for embedded use.
*/
static int os_time (lua_State *L) {
  time_t t;
  if (lua_isnoneornil(L, 1))  /* called without args? */
    t = time(NULL);  /* get current time */
  else {
    struct tm ts;
    luaL_checktype(L, 1, LUA_TTABLE);
    lua_settop(L, 1);  /* make sure table is at the top */
    
    memset(&ts, 0, sizeof(ts));
    
    /* Get the elements from the table */
    lua_getfield(L, 1, "year");
    ts.tm_year = (int)lua_tointeger(L, -1) - 1900;
    lua_getfield(L, 1, "month");
    ts.tm_mon = (int)lua_tointeger(L, -1) - 1;
    lua_getfield(L, 1, "day");
    ts.tm_mday = (int)lua_tointeger(L, -1);
    lua_getfield(L, 1, "hour");
    ts.tm_hour = (int)lua_tointeger(L, -1);
    lua_getfield(L, 1, "min");
    ts.tm_min = (int)lua_tointeger(L, -1);
    lua_getfield(L, 1, "sec");
    ts.tm_sec = (int)lua_tointeger(L, -1);
    
    t = mktime(&ts);
  }
  
  if (t == (time_t)(-1))
    return luaL_error(L, "time result cannot be represented in this system");
  else {
    lua_pushinteger(L, (lua_Integer)t);
    return 1;
  }
}


static int os_difftime (lua_State *L) {
  lua_pushnumber(L, difftime((time_t)(luaL_checknumber(L, 1)),
                            (time_t)(luaL_optnumber(L, 2, 0))));
  return 1;
}


/*
** Simplified date implementation for embedded use
*/
static int os_date (lua_State *L) {
  const char *s = luaL_optstring(L, 1, "%c");
  time_t t = luaL_opt(L, (time_t)luaL_checknumber, 2, time(NULL));
  struct tm tmr, *stm;
  
  stm = localtime(&t);
  if (stm == NULL)  /* invalid date? */
    return luaL_error(L, "time result cannot be represented in this system");
  
  tmr = *stm;  /* copy (just to be safe) */
  
  if (*s == '!') {  /* UTC? */
    stm = gmtime(&t);
    if (stm == NULL)
      return luaL_error(L, "time result cannot be represented in this system");
    tmr = *stm;
    s++;  /* skip '!' */
  }
  
  if (strcmp(s, "*t") == 0) {
    lua_createtable(L, 0, 9);  /* 9 = number of fields */
    
    /* Set fields */
    lua_pushinteger(L, tmr.tm_year + 1900);
    lua_setfield(L, -2, "year");
    lua_pushinteger(L, tmr.tm_mon + 1);
    lua_setfield(L, -2, "month");
    lua_pushinteger(L, tmr.tm_mday);
    lua_setfield(L, -2, "day");
    lua_pushinteger(L, tmr.tm_hour);
    lua_setfield(L, -2, "hour");
    lua_pushinteger(L, tmr.tm_min);
    lua_setfield(L, -2, "min");
    lua_pushinteger(L, tmr.tm_sec);
    lua_setfield(L, -2, "sec");
    lua_pushinteger(L, tmr.tm_wday + 1);
    lua_setfield(L, -2, "wday");
    lua_pushinteger(L, tmr.tm_yday + 1);
    lua_setfield(L, -2, "yday");
    
    /* pico doesn't use DST */
    lua_pushboolean(L, 0);
    lua_setfield(L, -2, "isdst");
  }
  else {
    /* Simplified for embedded use - just return a basic string */
    char buff[80];
    strftime(buff, sizeof(buff), s, &tmr);
    lua_pushstring(L, buff);
  }
  
  return 1;
}


static int os_getenv (lua_State *L) {
  /* No environment variables on RP2350 */
  lua_pushnil(L);
  return 1;
}


static int os_setlocale (lua_State *L) {
  /* Always return C locale on RP2350 */
  lua_pushstring(L, "C");
  return 1;
}


static int os_exit (lua_State *L) {
  /* On RP2350, we don't actually exit */
  if (lua_toboolean(L, 2))
    lua_close(L);
  return 0;
}


static const luaL_Reg syslib[] = {
  {"clock",     os_clock},
  {"date",      os_date},
  {"difftime",  os_difftime},
  {"exit",      os_exit},
  {"getenv",    os_getenv},
  {"setlocale", os_setlocale},
  {"time",      os_time},
  {NULL, NULL}
};


/* }====================================================== */


LUAMOD_API int luaopen_os (lua_State *L) {
  luaL_newlib(L, syslib);
  return 1;
}
