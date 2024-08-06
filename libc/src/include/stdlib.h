#ifndef _STDLIB_H 
#define _STDLIB_H 1

#include <stdint.h>

void itoa(uint64_t value, char* str);
void itoa_base(uint64_t value, char* str, int base);

#endif