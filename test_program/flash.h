#include "pico/stdlib.h"
#include "hardware/flash.h"
#include "hardware/sync.h"
#include <stdio.h>

// Define the size of the flash memory
#define FLASH_SIZE_BYTES (2 * 1024 * 1024) // 2MB of flash memory

// Define the start of the program in flash memory
#define FLASH_BASE_ADDR XIP_BASE

// (total_flash | used_flash | free_flash)
size_t check_flash_storage(int mode) {
    // Calculate total flash size
    size_t total_flash = FLASH_SIZE_BYTES;

    // Get the address of the end of the program
    extern uint8_t __flash_binary_end; // Provided by the linker
    size_t used_flash = (size_t)(&__flash_binary_end) - FLASH_BASE_ADDR;

    // Calculate free flash memory
    size_t free_flash = total_flash - used_flash;

    // Print flash storage information
    // printf("Total flash storage: %u bytes\n", total_flash);
    // printf("Used flash storage: %u bytes\n", used_flash);
    // printf("Free flash storage: %u bytes\n", free_flash);
    switch (mode) {
      case 0:
        return total_flash;
      case 1:
        return used_flash;
      case 2:
        return free_flash;
      default:
        printf("Invalid mode. Use 0 for total, 1 for used, or 2 for free.\n");
        return 0;
    }
}
