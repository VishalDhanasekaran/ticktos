#include <stdint.h>
#include "memory_map.h"

// Function pointer type for application entry point
typedef void (*app_entry_t)(void);

int main(void)
{
    uint32_t *app_code = (uint32_t*) __APPROM_START__;
    uint32_t app_sp    = app_code[0];  // Initial stack pointer
    uint32_t app_start = app_code[1];  // Reset handler address
    
    // Basic validation - check if application appears to be valid
    if (app_sp < 0x20000000 || app_sp > 0x20008000) 
    {
        // Invalid application - stay in bootloader
        while(1) {}
    }
    
    if ((app_start < (uint32_t)__APPROM_START__) || (app_start > ((uint32_t)__APPROM_START__ + (uint32_t)__APPROM_SIZE__)) || !(app_start & 1)) 
    {
        // Invalid application - stay in bootloader
        while(1) {}
    }
    
    // Disable interrupts before jumping
    __asm volatile ("cpsid i");
    
    // Set the stack pointer to the application's initial SP
    __asm volatile ("msr msp, %0" : : "r" (app_sp));
    
    // Jump to application
    app_entry_t app_entry = (app_entry_t)(app_start);
    app_entry();
    
    // Should never reach here
    while(1) {}
}
