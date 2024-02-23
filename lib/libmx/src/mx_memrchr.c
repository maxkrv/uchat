#include "libmx.h"

void *mx_memrchr(const void *s, int c, size_t n) {
    if (s == NULL
        || n == 0) {
        return NULL;
    }

    const unsigned char *str = (const unsigned char *) s + mx_strlen(s);

    while (n--) {
        if (*str == (unsigned char) c) {
            return (void *) str;
        }
        str--;
    }

    return NULL;
}
