#include "libmx.h"

char *mx_replace_substr(const char *str, const char *sub, const char *replace) {
    if (!str
        || !sub
        || !replace) {
        return NULL;
    }

    int substr_count = mx_count_substr(str, sub);
    int sub_size = mx_strlen(sub);
    int replace_size = mx_strlen(replace);

    char *result = mx_strnew(mx_strlen(str) + (replace_size - sub_size) * substr_count);
    char *tmp = result;

    for (int i = 0; i < substr_count; i++) {
        int substr_index = mx_get_substr_index(str, sub);

        tmp = mx_strncpy(tmp, str, mx_get_substr_index(str, sub)) + substr_index;
        tmp = mx_strcpy(tmp, replace) + replace_size;
        str += substr_index + sub_size;
    }

    mx_strcpy(tmp, str);

    return result;
}
