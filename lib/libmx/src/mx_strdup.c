#include "libmx.h"

char *mx_strdup(const char *str) {
    char *c = mx_strnew(mx_strlen(str));

    mx_strcpy(c, str);

    return c;
}

