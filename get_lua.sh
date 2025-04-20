#!/bin/bash
set -e

# Script to download and build Lua for RP2350 (Pico 2)
# The Lua source and build files will be in the ./lib/lua directory

# Create lib/lua directory if it doesn't exist
mkdir -p lib/lua
cd lib/lua

# Download Lua source code (using Lua 5.4.6 which is the latest stable version)
if [ ! -f lua-5.4.6.tar.gz ]; then
    echo "Downloading Lua 5.4.6..."
    wget http://www.lua.org/ftp/lua-5.4.6.tar.gz
fi

# Extract Lua source code if not already extracted
if [ ! -d lua-5.4.6 ]; then
    echo "Extracting Lua source code..."
    tar xzf lua-5.4.6.tar.gz
fi

cd lua-5.4.6

# Cross-compilation setup for RP2350
# Setting up environment for ARM GCC cross-compiler 
# Using pico-sdk for toolchain settings

# Source environment variables from env.src if it exists
if [ -f ../../../env.src ]; then
    source ../../../env.src
fi

# Determine cross-compiler
if [ -z "$PICO_TOOLCHAIN_PATH" ]; then
    # Try to find arm-none-eabi-gcc in path
    if command -v arm-none-eabi-gcc &> /dev/null; then
        CROSS_COMPILE=arm-none-eabi-
    else
        echo "Error: arm-none-eabi-gcc not found. Please set PICO_TOOLCHAIN_PATH in env.src"
        exit 1
    fi
else
    CROSS_COMPILE="${PICO_TOOLCHAIN_PATH}/bin/arm-none-eabi-"
fi

echo "Using cross compiler: ${CROSS_COMPILE}gcc"

# Create a patches directory for any needed modifications
mkdir -p ../patches

# Instead of using a patch, directly create a modified version of loslib.c
echo "Creating modified loslib.c for RP2350..."

# Create a backup of the original file
cp src/loslib.c src/loslib.c.orig

# Create the modified file
cat > src/loslib.c << 'EOF'
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
EOF

# Modify the makefile to cross-compile for RP2350
echo "Creating custom makefile for RP2350 cross-compilation..."
cat > Makefile.rp2350 << EOF
# Makefile for building Lua for RP2350
# Adapted from the original Lua Makefile

CC=${CROSS_COMPILE}gcc
AR=${CROSS_COMPILE}ar
RANLIB=${CROSS_COMPILE}ranlib
LD=${CROSS_COMPILE}ld
SIZE=${CROSS_COMPILE}size
STRIP=${CROSS_COMPILE}strip

CFLAGS= -DLUA_USE_LONGJMP -DLUA_32BITS -Os -mcpu=cortex-m0plus -mthumb -ffunction-sections -fdata-sections -Wall -Wextra -nostdlib -ffreestanding -DLUA_COMPAT_5_3
LDFLAGS= -Wl,--gc-sections

CORE_O=lapi.o lcode.o lctype.o ldebug.o ldo.o ldump.o lfunc.o lgc.o llex.o lmem.o lobject.o lopcodes.o lparser.o lstate.o lstring.o ltable.o ltm.o lundump.o lvm.o lzio.o
LIB_O=lauxlib.o lbaselib.o lcorolib.o ldblib.o liolib.o lmathlib.o loadlib.o loslib.o lstrlib.o ltablib.o lutf8lib.o linit.o

ALL_O= \$(CORE_O) \$(LIB_O)
ALL_A= liblua.a

all: \$(ALL_A)

\$(ALL_A): \$(ALL_O)
	\$(AR) rcu \$@ \$(ALL_O)
	\$(RANLIB) \$@

clean:
	rm -f \$(ALL_A) \$(ALL_O)

.c.o:
	\$(CC) \$(CFLAGS) -c \$<
EOF

# Build Lua for RP2350
echo "Building Lua for RP2350..."
# Need to specify source directory for the .c files
cd src
make -f ../Makefile.rp2350

# Create a header directory to make it easier to include in other projects
echo "Creating headers directory..."
cd ..
mkdir -p include
find src -name "*.h" -exec cp {} include/ \;

# Copy the compiled library to the lib directory
echo "Copying compiled library..."
cp src/liblua.a .

echo "Lua for RP2350 has been successfully built."
echo "Static library is available at: lib/lua/liblua.a"
echo "Headers are available at: lib/lua/include/"

cd ../../..