#ifndef RAMDISK_H
#define RAMDISK_H

#include "../utils/types.h"

// Define Ramdisk size and location
// For this demo, we'll use a fixed buffer in the kernel data section
// In a real OS, this would be loaded by the bootloader to a specific address
#define RAMDISK_SIZE (1024 * 10) // 10KB Ramdisk

void ramdisk_init(const uint8_t* key);
void ramdisk_read(size_t offset, size_t size, uint8_t* buffer);
void ramdisk_write(size_t offset, size_t size, const uint8_t* buffer);

// For demo purposes, we expose the raw pointer to show it's encrypted
const uint8_t* ramdisk_get_raw_pointer();

#endif
