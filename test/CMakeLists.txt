cmake_minimum_required(VERSION 3.13)

# Include the Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Set the project name and C standard
project(test_project C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the Pico SDK
pico_sdk_init()

# Add the executable
add_executable(test
        test.c
)

# Pull in common dependencies
target_link_libraries(test pico_stdlib)

# Enable USB output, disable UART output
pico_enable_stdio_usb(test 1)
pico_enable_stdio_uart(test 0)

# Create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(test)

# Set the program URL for picotool
# example_auto_set_url(test)

