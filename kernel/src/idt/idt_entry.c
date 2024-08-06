#include "src/include/stdio.h"
#include "../include/idt.h"

static idtr_t idtr; //(used to make the idt pointer)

// create the table array
__attribute__((aligned(0x10)));
static idt_entry_t idt[256];

void set_idt_descriptor(uint8_t vector, void* isr, uint8_t flags);
void set_idt_descriptor(uint8_t vector, void* isr, uint8_t flags){
    idt_entry_t* descriptor = &idt[vector];
    
    descriptor->low_isr     = (uint64_t)isr & 0xFFFF;
    descriptor->kernel_cs   = 0x08;
    descriptor->ist         = 0;
    descriptor->attributes  = flags;
    descriptor->mid_isr     = ((uint64_t)isr >> 16) & 0xFFFF;
    descriptor->high_isr    = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
    descriptor->reserved    = 0;
}

static bool vectors[sizeof(idt)];

extern void* isr_stub_table[];

void init_idt(void);
void init_idt(){
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * sizeof(idt) - 1;

    for (uint8_t vector = 0; vector < 32; vector++){
        set_idt_descriptor(vector, isr_stub_table[vector], 0x8E);
        vectors[vector] = true;
    }

    __asm__ volatile("lidt %0" : : "m"(idtr));
    __asm__ volatile("sti");
}