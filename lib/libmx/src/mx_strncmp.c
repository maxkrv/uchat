#include "libmx.h"

int mx_strncmp(const char *s1, const char *s2, int n) {
    const char *end = s1 + n;
    int result = 0;

    for (; result == 0 && s1 != end && (*s1 || *s2); result = *(s1++) - *(s2++));

    return result;
}



