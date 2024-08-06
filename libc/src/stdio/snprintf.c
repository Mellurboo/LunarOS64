#include <include/string.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>

int snprintf(char *buffer, size_t size, const char *format, ...) {
    va_list args;
    va_start(args, format);

    size_t pos = 0;
    const char *ptr = format;

    while (*ptr) {
        if (*ptr == '%' && (*(ptr + 1) == 's'
        || *(ptr + 1) == 'd'
        || *(ptr + 1) == 'c'
        || (*(ptr + 1) == 'l' && *(ptr + 2) == 'l' && *(ptr + 3) == 'd')
        || *(ptr + 1) == 'x'
        || *(ptr + 1) == 'p')) {
            ptr++; // Skip '%'
            int width = 0;
            int precision = -1;
            
            // Handle width
            if (*ptr >= '0' && *ptr <= '9') {
                width = 0;
                while (*ptr >= '0' && *ptr <= '9') {
                    width = width * 10 + (*ptr - '0');
                    ptr++;
                }
            }

            // Handle precision (only for floating-point numbers, not handled here)
            if (*ptr == '.') {
                ptr++;
                precision = 0;
                while (*ptr >= '0' && *ptr <= '9') {
                    precision = precision * 10 + (*ptr - '0');
                    ptr++;
                }
            }

            // Format specifier handling
            if (*ptr == 's') {
                const char *str = va_arg(args, const char *);
                size_t len = strlen(str);

                // Apply width
                size_t padding = (width > len) ? (width - len) : 0;
                for (size_t i = 0; i < padding && pos < size - 1; i++) {
                    buffer[pos++] = ' ';
                }
                for (size_t i = 0; i < len && pos < size - 1; i++) {
                    buffer[pos++] = str[i];
                }
            } else if (*ptr == 'd') {
                int num = va_arg(args, int);
                char num_str[21]; // Enough to hold any 64-bit integer
                itoa_base((uint64_t)num, num_str, 10);

                // Apply width
                size_t len = strlen(num_str);
                size_t padding = (width > len) ? (width - len) : 0;
                for (size_t i = 0; i < padding && pos < size - 1; i++) {
                    buffer[pos++] = ' ';
                }
                for (size_t i = 0; i < len && pos < size - 1; i++) {
                    buffer[pos++] = num_str[i];
                }
            } else if (*ptr == 'c') {
                char ch = (char)va_arg(args, int);
                if (pos < size - 1) {
                    buffer[pos++] = ch;
                }
            } else if (*ptr == 'l' && *(ptr + 1) == 'l' && *(ptr + 2) == 'd') {
                ptr += 2; // Skip 'lld'
                long long int num = va_arg(args, long long int);
                char num_str[21]; // 64-bit integer
                itoa_base((uint64_t)num, num_str, 10);

                // Apply width
                size_t len = strlen(num_str);
                size_t padding = (width > len) ? (width - len) : 0;
                for (size_t i = 0; i < padding && pos < size - 1; i++) {
                    buffer[pos++] = ' ';
                }
                for (size_t i = 0; i < len && pos < size - 1; i++) {
                    buffer[pos++] = num_str[i];
                }
            } else if (*ptr == 'x') {
                uint32_t num = va_arg(args, uint32_t);
                char num_str[9]; // Enough to hold a 32-bit hex number + null terminator
                itoa_base(num, num_str, 16);

                // Apply width
                size_t len = strlen(num_str);
                size_t padding = (width > len) ? (width - len) : 0;
                for (size_t i = 0; i < padding && pos < size - 1; i++) {
                    buffer[pos++] = ' ';
                }
                for (size_t i = 0; i < len && pos < size - 1; i++) {
                    buffer[pos++] = num_str[i];
                }
            } else if (*ptr == 'p') {
                void *ptr_val = va_arg(args, void *);
                uintptr_t addr = (uintptr_t)ptr_val;
                char addr_str[19]; // 16 characters for 64-bit hex + "0x" + null terminator
                addr_str[0] = '0';
                addr_str[1] = 'x';
                itoa_base(addr, addr_str + 2, 16);
                size_t len = strlen(addr_str);

                // Apply width
                size_t padding = (width > len) ? (width - len) : 0;
                for (size_t i = 0; i < padding && pos < size - 1; i++) {
                    buffer[pos++] = ' ';
                }
                for (size_t i = 0; i < len && pos < size - 1; i++) {
                    buffer[pos++] = addr_str[i];
                }
            }
        } else {
            if (pos < size - 1) {
                buffer[pos++] = *ptr;
            }
        }
        ptr++;
    }

    buffer[pos] = '\0'; // Null-terminate the string
    va_end(args);

    return pos;
}