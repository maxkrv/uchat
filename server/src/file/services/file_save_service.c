#include "server.h"

static t_string form_url(char *filename) {
    t_env_params *params = mx_env_get();
    t_string url = mg_mprintf("http://%s:%d/assets/uploads/%s", params->domain,
                              params->port, filename);

    return url;
}

t_file *mx_file_save_to_db(char *filename) {
    t_string url = form_url(filename);

    int file_id = mx_file_repo_create(filename, url);
    mx_strdel(&url);
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
