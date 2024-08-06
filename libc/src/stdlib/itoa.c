#include "../include/stdlib.h"

void itoa_base(uint64_t value, char* str, int base) {
    char buffer[65]; 
    int pos = 0;

    if (value == 0) {
        str[pos++] = '0';
        str[pos] = '\0';
        return;
    }

    while (value > 0) {
        int digit = value % base;
        buffer[pos++] = (digit < 10) ? ('0' + digit) : ('a' + (digit - 10));
        value /= base;
    }

    // flip
    for (int i = 0; i < pos; i++) {
        str[i] = buffer[pos - 1 - i];
    }

    str[pos] = '\0';
}

void itoa(uint64_t value, char* str) {
    itoa_base(value, str, 10);
}