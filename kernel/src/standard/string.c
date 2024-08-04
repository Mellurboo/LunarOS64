#include <stdint.h>

int strlen(char *str) {
    int len = 0;
    while (*str++) len++;
    return len;
}

// Helper function to convert integer to string in a specific base
void itoa_base(uint64_t value, char* str, int base) {
    char buffer[65]; // Enough to hold binary representation of a 64-bit number + null terminator
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

    // Reverse the string
    for (int i = 0; i < pos; i++) {
        str[i] = buffer[pos - 1 - i];
    }

    str[pos] = '\0';
}

void itoa(uint64_t value, char* str) {
    itoa_base(value, str, 10);
}