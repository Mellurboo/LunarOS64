#ifndef _SYSINFO_H
#define _SYSINFO_H 1

#include <stdint.h>
#include <stddef.h>

static uint64_t getInstalledMemory();
void printInstalledMemory();

int getCPUCount();
void printCPUCount();

#endif