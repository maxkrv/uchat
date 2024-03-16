#include "server.h"

static t_string form_url(char *filename) {
    t_env_params *params = mx_env_get();
    t_string uri = mx_path_join(MX_UPLOAD_DIR_PATH, filename);
    t_string tmp = mx_strjoin("http://", params->domain);
    t_string port = mx_itoa(params->port);
    t_string tmp2 = mx_strjoin(tmp, port);
    t_string url = mx_strjoin(tmp2, uri);

    mx_strdel(&uri);
    mx_strdel(&port);
    mx_strdel(&tmp);
    mx_strdel(&tmp2);

    return url;
}

t_file *mx_file_save_to_db(char *filename) {
    t_string url = form_url(filename);

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
