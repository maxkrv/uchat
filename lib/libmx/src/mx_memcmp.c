#include "libmx.h"

int mx_memcmp(const void *s1, const void *s2, size_t n) {
    const unsigned char *f = (const unsigned char *) s1;
    const unsigned char *s = (const unsigned char *) s2;

    for (size_t i = 0; i < n; i++) {
        if (f[i] != s[i]) {
            return (int) (f[i] - s[i]);
        }
    }

    return 0;
}
