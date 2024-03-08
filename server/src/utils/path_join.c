#include "utils.h"

char *mx_path_join(const char *path, const char *name) {
    char *tmp = mx_strjoin(path, "/");
    char *res = mx_strjoin(tmp, name);

    mx_strdel(&tmp);

    return res;
}
