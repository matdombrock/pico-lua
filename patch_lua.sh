#!/bin/bash
set -e

# Script to download and build Lua for RP2350 (Pico 2)
# The Lua source and build files will be in the ./lib/lua directory

top_lvl=../../..

# Create lib/lua directory if it doesn't exist
mkdir -p lib/lua
rm -rf lib/lua/*
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
if [ -f ${top_lvl}/env.src ]; then
  source ${top_lvl}/env.src
fi

# Determine cross-compiler
if [ -z "$PICO_TOOLCHAIN_PATH" ]; then
  # Try to find arm-none-eabi-gcc in path
  if command -v arm-none-eabi-gcc &>/dev/null; then
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
cp ${top_lvl}/lua_patches/loslib.c src/loslib.c
cp ${top_lvl}/lua_patches/luaconf.h src/luaconf.h

# Modify the makefile to cross-compile for RP2350
echo "Creating custom makefile for RP2350 cross-compilation..."
# cat > Makefile.rp2350 << EOF
cp ${top_lvl}/lua_patches/Makefile.rp2350 Makefile.rp2350

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
find src -name "*.hpp" -exec cp {} include/ \;

# Copy the compiled library to the lib directory
echo "Copying compiled library..."
cp src/liblua.a .

echo "Lua for RP2350 has been successfully built."
echo "Static library is available at: ./lib/lua/lua-5.4.6/liblua.a"
echo "Headers are available at: ./lib/lua/lua-5.4.6/include/"

cd ../../..
