#include "server.h"

t_file *mx_file_save_to_db(char *filename, char *url) {
    int file_id = mx_file_repo_create(filename, url);
    return mx_file_repo_get(file_id);
}

t_file *mx_file_delete_from_db(int file_id) {
    t_file *file = mx_file_repo_get(file_id);

    if (!file) {
        return NULL;
    }

    if (!mx_file_repo_delete(file_id)) {
        mx_file_free(file);
        return NULL;
    };

    return file;
}
