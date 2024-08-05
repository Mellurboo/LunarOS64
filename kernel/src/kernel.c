// standard limine initialisation

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limine.h>

#include <libc/include/stdio.h>
#include <include/string.h>
#include <libc/include/string.h>
#include <include/arch64/gdt.h>

#include "kernel.h"

// Limine version
__attribute__((used, section(".requests")))
static volatile LIMINE_BASE_REVISION(2);

// Request the memory map
__attribute__((used, section(".requests")))
static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

__attribute__((used, section(".requests")))
static volatile struct limine_smp_request smp_request = {
    .id = LIMINE_SMP_REQUEST,
    .revision = 0
};

// start and end markers for the Limine requests.
__attribute__((used, section(".requests_start_marker")))
static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".requests_end_marker")))
static volatile LIMINE_REQUESTS_END_MARKER;


static void halt(void) {
  asm ("cli");
  for (;;) {
      asm ("hlt");
  }
}

// here it is, the boot 
void kstart(void) {
  video_init();

  // ==== BOOT CHECKS ==== //

  if (LIMINE_BASE_REVISION_SUPPORTED == false
  || memmap_request.response == NULL) {
    halt();
  }

  kprintf("%sLunar OS - x64 UEFI%s\n", BYEL, reset);

  // ==== Calculating Total Installed Memory ==== /
  uint64_t total_memory = 0;
  for (size_t i = 0; i < memmap_request.response->entry_count; i++) {
    struct limine_memmap_entry *entry = memmap_request.response->entries[i];
    
    if (entry->type == LIMINE_MEMMAP_USABLE || entry->type == LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE) {
        total_memory += entry->length;
    }
  }

  char memory_str[21];
  itoa((total_memory / BYTE_TO_MB), memory_str);
  kprintf("%s Physical Memory: %s MiB\n", SYSTEM, memory_str);

  char cpus[21];
  itoa(smp_request.response->cpu_count, cpus, 10);
  kprintf("%s CPUs Detected: %s\n", SYSTEM, cpus);

  init_gdt();
  
  halt();
}
