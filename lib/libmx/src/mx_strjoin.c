#include "libmx.h"

char *mx_strjoin(char const *s1, char const *s2) {
    if (s1 == NULL) {
        return s2 == NULL ? NULL : mx_strdup(s2);
    } else if (s2 == NULL) {
        return mx_strdup(s1);
    }

    char *result = mx_strnew(mx_strlen(s1) + mx_strlen(s2));

    mx_strcpy(result, s1);
    mx_strcat(result, s2);

    return result;
}

