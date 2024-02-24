#include "libmx.h"

char *mx_strnew(const int size) {
    char *c = malloc(size + 1);

    if (size < 0)
        return NULL;

    for (int i = 0; i <= size; ++i) {
        *(c + i) = '\0';
    }

    return c;
}
