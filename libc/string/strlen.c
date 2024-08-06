#include <include/string.h>

int stringlen(char *str) {
    int len = 0;
    while (*str++) len++;
    return len;
}