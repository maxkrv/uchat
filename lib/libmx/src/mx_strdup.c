#include "libmx.h"

char *mx_strdup(const char *str) {
    if (str == NULL) {
        return NULL;
    }
    char *c = mx_strnew(mx_strlen(str));

    mx_strcpy(c, str);

    return c;
}
