#include "libmx.h"

void *mx_memccpy(void *restrict dst, const void *restrict src, int c, size_t n) {
    unsigned char *source = (unsigned char *) src;
    unsigned char *destination = (unsigned char *) dst;

    for (size_t i = 0; i < n; i++) {
        destination[i] = source[i];

        if (source[i] == (unsigned char) c) {
            return &destination[i + 1]; // Return a pointer to the byte after the one copied
        }
    }

    return NULL; // Character not found in 'n' bytes
}
