#include <stdint.h>

typedef struct idt_entry{
    uint16_t    low_isr;
    uint16_t    kernel_cs;
    uint8_t     ist;
    uint8_t     attributes;
    uint16_t    mid_isr;
    uint16_t    high_isr;
    uint32_t    reserved;
}__attribute__((packed)) idt_entry_t;

typedef struct idtr{
    uint16_t    limit;
    uint64_t    base;
}__attribute__((packed)) idtr_t;

void init_idt(void);