#include "libmx.h"

void *mx_memcpy(void *restrict dst, const void *restrict src, size_t n) {
    unsigned char *source = (unsigned char *) src;
    unsigned char *destination = (unsigned char *) dst;

    for (size_t i = 0; i < n; i++) {
        destination[i] = source[i];
    }

    return dst;
}
