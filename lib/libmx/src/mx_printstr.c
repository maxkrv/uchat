#include "libmx.h"

void mx_printstr(const char *s) {
    write(STDOUT_FILENO, s, mx_strlen(s));
}
