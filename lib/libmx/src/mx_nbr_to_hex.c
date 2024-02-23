#include "libmx.h"

static char *strnew(const int size) {
    char *c = malloc(size + 1);

    if (size < 0)
        return NULL;

    for (int i = 0; i <= size; ++i) {
        *(c + i) = '\0';
    }

    return c;
}

static int get_len(unsigned long temp) {
    int len = 0;

    while (temp != 0) {
        len++;
        temp /= 16;
    }

    return len;
}

char *mx_nbr_to_hex(unsigned long nbr) {
    int len = get_len(nbr);
    char *c = strnew(len);

    for (int i = len - 1; i >= 0; --i) {
        unsigned int t = nbr % 16;

        if (t >= 10) {
            c[i] = 87 + t;
        } else {
            c[i] = 48 + t;
        }

        nbr /= 16;
    }

    return c;
}

