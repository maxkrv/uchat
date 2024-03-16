#include "server.h"

bool mx_file_upload(const char *file, int file_len, const char *filename) {
    t_string file_path = mx_path_join(mx_env_get()->upload_dir, filename);
    FILE *fptr = fopen(file_path, "r");

    if (fptr) {
        mx_strdel(&file_path);
        fclose(fptr);
        return false;
    }

    fptr = fopen(file_path, "wb+");

    mx_strdel(&file_path);

    if (!fptr) {
        return false;
    }
    fwrite(file, file_len, 1, fptr);
    fclose(fptr);

    return true;
}

bool mx_file_delete_uploaded(char *filename) {
    t_string file_path = mx_path_join(mx_env_get()->upload_dir, filename);
    FILE *fptr = fopen(file_path, "r");

    if (fptr) {
        mx_strdel(&file_path);
        fclose(fptr);
        return false;
    }

    bool res = remove(file_path) == 0;

    mx_strdel(&file_path);

    return res;
}
