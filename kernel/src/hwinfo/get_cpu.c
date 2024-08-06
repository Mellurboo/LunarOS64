#include "../include/limine.h"
#include "src/include/stdio.h"

__attribute__((used, section(".requests")))
static volatile struct limine_smp_request smp_request = {
    .id = LIMINE_SMP_REQUEST,
    .revision = 0
};

int getCPUCount(){
    return smp_request.response->cpu_count;
}

void printCPUCount(){
    const char cpus[21];
    itoa(getCPUCount(), cpus);
    kprintf("%s CPUs Detected: %s\n", SYSTEM, cpus);
}