#include "../include/string.h"

int tostring(char *str) {
    int len = 0;
    while (*str++) len++;
    return len;
}