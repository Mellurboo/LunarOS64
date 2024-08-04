#include <stdint.h>

#include <include/arch64/des_tables/gdt.h>
#include <include/stdio.h>
#include <include/string.h>

#pragma pack(push, 1)  // Ensure no padding

// Define a GDT descriptor entry
typedef struct {
    uint16_t low_limit;    // Lower 16 bits of the limit
    uint16_t low_base;     // Lower 16 bits of the base address
    uint8_t  mid_base;  // Next 8 bits of the base address
    uint8_t  access;       // Access flags
    uint8_t  granularity;  // Granularity and high bits of limit
    uint8_t  high_base;    // Highest 8 bits of the base address
} __attribute__((packed)) gdt_entry_t;

// Define a GDT pointer structure
typedef struct {
    uint16_t limit;    // Size of the GDT - 1
    uint64_t base;     // Base address of the GDT
} __attribute__((packed)) gdt_ptr_t;

// GDT entries
gdt_entry_t gdt_entries[6];
gdt_ptr_t gdt_ptr;

void setup_gdt_entry(gdt_entry_t *entry, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
  entry->low_base = (base & 0xFFFF);
  entry->mid_base = (base >> 16) & 0xFF;
  entry->high_base = (base >> 24) & 0xFF;
  entry->low_limit = (limit & 0xFFFF);
  entry->granularity = ((limit >> 16) & 0x0F) | (gran & 0xF0);
  entry->access = access;
}

void print_gdt_descriptor(const gdt_entry_t* desc) {
    kprintf("\tBase Address: %x\n",
            desc->high_base, desc->mid_base, desc->low_base, 0);
    kprintf("\tLimit: 0x%x\n", (desc->granularity & 0x0F) << 16 | desc->low_limit);
    kprintf("\tAccess: 0x%x\n", desc->access);
    kprintf("\tGranularity: 0x%x\n", desc->granularity);
}

void init_gdt() {
  setup_gdt_entry(&gdt_entries[0], 0, 0, 0, 0);                 // Null Descriptor

  setup_gdt_entry(&gdt_entries[1], 0, 0xFFFFFFFF, 0x9A, 0xCF);  // Kernel Code Segment
  setup_gdt_entry(&gdt_entries[2], 0, 0xFFFFFFFF, 0x92, 0xCF);  // Kernel Data Segment
  setup_gdt_entry(&gdt_entries[3], 0, 0xFFFFFFFF, 0xFA, 0xCF);  // User Code Segment
  setup_gdt_entry(&gdt_entries[4], 0, 0xFFFFFFFF, 0xF2, 0xCF);  // User Data Segment
  setup_gdt_entry(&gdt_entries[5], 0, 0x67, 0x89, 0x40);        // TSS (not used for now)

  gdt_ptr.limit = (sizeof(gdt_entry_t) * 6) - 1;
  gdt_ptr.base = (uint64_t)&gdt_entries;

  kprintf("%s GDT Loaded: 0x%x\n", INFO, &gdt_ptr);

  kprintf("%sNull: %s\n", YEL, reset);
  print_gdt_descriptor(&gdt_entries[0]);
  kprintf("%sKernel Code: %s\n", RED, reset);
  print_gdt_descriptor(&gdt_entries[1]);
  kprintf("%sKernel Data: %s\n", RED, reset);
  print_gdt_descriptor(&gdt_entries[2]);
  kprintf("%sUser Code: %s\n", GRN, reset);
  print_gdt_descriptor(&gdt_entries[3]);
  kprintf("%sUser Data: %s\n", GRN, reset);
  print_gdt_descriptor(&gdt_entries[4]);
  kprintf("%sTSS %s\n", BLU, reset);
  print_gdt_descriptor(&gdt_entries[5]);

  load_gdt();
  reload_segments();
}

void load_gdt() {
  asm volatile (
    "lgdt (%0)"
    :
    : "r" (&gdt_ptr)
    : "memory"
  );
}