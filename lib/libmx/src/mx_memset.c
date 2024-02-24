#include "libmx.h"

void *mx_memset(void *b, int c, size_t len) {
    unsigned char *p = (unsigned char *) b;
    unsigned char value = (unsigned char) c; // Ensure it's treated as an unsigned char

    for (size_t i = 0; i < len; i++) {
        p[i] = value;
    }

    return b;
}
