#ifndef _STRING_H 
#define _STRING_H 1

#include <stdint.h>

int strlen(const char content[]);
void itoa_base(uint64_t value, char* str, int base);
void itoa(uint64_t value, char* str);

#endif