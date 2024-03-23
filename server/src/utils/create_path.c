#include "base.h"

bool mx_create_path_if_not_exist(char *path) {
    FILE *file = fopen(path, "r");
    if (file) {
        fclose(file);
        return true;
    }

    return mkdir(path, 0777) == 0;
}
