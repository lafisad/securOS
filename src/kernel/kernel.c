#include "utils/types.h"

void kernel_main() {
    // Write directly to VGA memory - no drivers, no initialization
    volatile uint16_t* vga = (volatile uint16_t*)0xB8000;
    
    // Clear screen to black first
    for(int i = 0; i < 80 * 25; i++) {
        vga[i] = 0x0000;
    }
    
    // Write message
    const char* msg = "securOS BOOT OK";
    uint8_t color = 0x0A;  // Light green on black
    
    for(int i = 0; msg[i]; i++) {
        vga[i] = (color << 8) | msg[i];
    }
    
    // Halt
    while(1) {
        __asm__ volatile("hlt");
    }
}
