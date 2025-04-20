cmake_minimum_required(VERSION 3.13)

# Include the Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Set the project name and C standard
project(test_project C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the Pico SDK
pico_sdk_init()

# Set path to Lua library and include directories
set(LUA_DIR "${CMAKE_SOURCE_DIR}/../lib/lua/lua-5.4.6/")
set(LUA_INCLUDE_DIR "${LUA_DIR}/include")
set(LUA_LIBRARY_DIR "${LUA_DIR}")

# Add the executable
add_executable(test
        test.c
)

# Include directories for lua headers
target_include_directories(test PRIVATE
    ${LUA_INCLUDE_DIR}
)

# Link against the static Lua library
target_link_directories(test PRIVATE
    ${LUA_LIBRARY_DIR}
)

# Pull in dependencies
target_link_libraries(test 
    pico_stdlib
    liblua.a
)

# Enable USB output, disable UART output
pico_enable_stdio_usb(test 1)
pico_enable_stdio_uart(test 0)

# Create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(test)

# Set the program URL for picotool
# example_auto_set_url(test)

