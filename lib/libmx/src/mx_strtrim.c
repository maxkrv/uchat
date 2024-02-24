#include "libmx.h"

char *mx_strtrim(const char *str) {
    if (str == NULL) {
        return NULL;
    }

    int start = 0;
    int end = mx_strlen(str) - 1;

    while (mx_isspace(str[start])) start++;
    while (mx_isspace(str[end])) end--;

    if (start > mx_strlen(str) || end < 0) {
        return mx_strdup("");
    }

    char *result = mx_strnew(end - start + 1);

    mx_strncpy(result, &str[start], end - start + 1);

    return result;
}
