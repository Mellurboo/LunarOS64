#include <limine.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include <include/string.h>

// Request the framebuffer 
__attribute__((used, section(".requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

struct flanterm_context *flt_cont;

// Function to initialize video (dummy implementation for demonstration)
void video_init() {
    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

    flt_cont = flanterm_fb_init(
        NULL, 
        NULL,
        framebuffer->address,           framebuffer->width, framebuffer->height, framebuffer->pitch,
        framebuffer->red_mask_size,     framebuffer->red_mask_shift,
        framebuffer->green_mask_size,   framebuffer->green_mask_shift,
        framebuffer->blue_mask_size,    framebuffer->blue_mask_shift,
        NULL,
        NULL, NULL,
        NULL, NULL,
        NULL, NULL,
        NULL, 0, 0, 1,
        0, 0,
        0
    );
}

// Function to format and print to the framebuffer
void kprintf(const char* format, ...) {
    va_list args;
    va_start(args, format);

    char buffer[1024]; // Buffer to store the final formatted string
    size_t buffer_index = 0;

    for (size_t i = 0; format[i] != '\0'; i++) {
        if (format[i] == '%' && format[i + 1] != '\0') {
            i++;
            int width = 0;
            int precision = -1;
            
            // Handle width
            if (format[i] >= '0' && format[i] <= '9') {
                width = 0;
                while (format[i] >= '0' && format[i] <= '9') {
                    width = width * 10 + (format[i] - '0');
                    i++;
                }
            }

            // Handle precision (only for floating-point numbers, not handled here)
            if (format[i] == '.') {
                i++;
                precision = 0;
                while (format[i] >= '0' && format[i] <= '9') {
                    precision = precision * 10 + (format[i] - '0');
                    i++;
                }
            }

            // Format specifier handling
            if (format[i] == 'd') {
                int num = va_arg(args, int);
                char num_str[32];
                itoa_base((uint64_t)num, num_str, 10);

                // Apply width
                size_t len = strlen(num_str);
                size_t padding = (width > len) ? (width - len) : 0;
                for (size_t j = 0; j < padding && buffer_index < sizeof(buffer) - 1; j++) {
                    buffer[buffer_index++] = ' ';
                }
                for (size_t j = 0; j < len && buffer_index < sizeof(buffer) - 1; j++) {
                    buffer[buffer_index++] = num_str[j];
                }
            } else if (format[i] == 's') {
                const char* str = va_arg(args, const char*);
                size_t len = strlen(str);

                // Apply width
                size_t padding = (width > len) ? (width - len) : 0;
                for (size_t j = 0; j < padding && buffer_index < sizeof(buffer) - 1; j++) {
                    buffer[buffer_index++] = ' ';
                }
                for (size_t j = 0; j < len && buffer_index < sizeof(buffer) - 1; j++) {
                    buffer[buffer_index++] = str[j];
                }
            } else if (format[i] == 'x') {
                uint32_t num = va_arg(args, uint32_t);
                char num_str[32];
                itoa_base(num, num_str, 16);

                // Apply width
                size_t len = strlen(num_str);
                size_t padding = (width > len) ? (width - len) : 0;
                for (size_t j = 0; j < padding && buffer_index < sizeof(buffer) - 1; j++) {
                    buffer[buffer_index++] = ' ';
                }
                for (size_t j = 0; j < len && buffer_index < sizeof(buffer) - 1; j++) {
                    buffer[buffer_index++] = num_str[j];
                }
            } else if (format[i] == 'p') {
                void *ptr_val = va_arg(args, void *);
                uintptr_t addr = (uintptr_t)ptr_val;
                char addr_str[19]; // 16 characters for 64-bit hex + "0x" + null terminator
                addr_str[0] = '0';
                addr_str[1] = 'x';
                itoa_base(addr, addr_str + 2, 16);
                size_t len = strlen(addr_str);

                // Apply width
                size_t padding = (width > len) ? (width - len) : 0;
                for (size_t j = 0; j < padding && buffer_index < sizeof(buffer) - 1; j++) {
                    buffer[buffer_index++] = ' ';
                }
                for (size_t j = 0; j < len && buffer_index < sizeof(buffer) - 1; j++) {
                    buffer[buffer_index++] = addr_str[j];
                }
            } else {
                buffer[buffer_index++] = '%';
                buffer[buffer_index++] = format[i];
            }
        } else {
            buffer[buffer_index++] = format[i];
        }
    }

    buffer[buffer_index] = '\0'; // Null-terminate the final string
    flanterm_write(flt_cont, buffer, buffer_index);

    va_end(args);
}

// Function to format a string with a maximum length
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
