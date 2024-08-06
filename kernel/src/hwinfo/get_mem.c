#include "../include/limine.h"
#include "../include/sysinfo.h"
#include "../include/panic.h"

#include "src/include/stdio.h"
#include "src/include/string.h"

#define BYTE_TO_KB 1024
#define BYTE_TO_MB 1048576
#define BYTE_TO_GB 1073741824

__attribute__((used, section(".requests")))
static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

/// @brief itterates across the memory map to get all discoverable memory
/// @return physical memory in bytes
static uint64_t getInstalledMemory(){
    if (memmap_request.response == NULL) {
        kprintf("%s The Kernel has decided to halt as it can't continue.", ERROR);

        asm ("cli");
        for (;;) {
            asm ("hlt");
        }
    }

    uint64_t m = 0;
    for (size_t i = 0; i < memmap_request.response->entry_count; i++) {
        struct limine_memmap_entry *entry = memmap_request.response->entries[i];
        
        if (entry->type == LIMINE_MEMMAP_USABLE || entry->type == LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE) {
            m += entry->length;
        }
    }

    return m;
}

/// @brief prints the installed memory in MiB
void printInstalledMemory(){
    const char mem_info[21];
    itoa(getInstalledMemory() / BYTE_TO_MB, mem_info);
    kprintf("%s Physical Memory: %sMiB\n", SYSTEM, mem_info);
}

