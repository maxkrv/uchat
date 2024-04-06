#include "server.h"

t_file *mx_file_service_get(int file_id) {
    return mx_file_repo_get(file_id);
}

static t_string form_url(char *filename) {
    t_env_params *params = mx_env_get();
    t_string url = mg_mprintf("http://%s:%d/assets/uploads/%s", params->domain,
                              params->port, filename);

    return url;
}

t_file *mx_file_service_upload(char *file, int file_len, char *filename) {
    t_string uuid = mx_gen_uuid();
    t_string uuid_filename = mg_mprintf("%s-%s", uuid, filename);

    mx_strdel(&uuid);

    if (!mx_file_upload(file, file_len, uuid_filename)) {
        mx_strdel(&uuid_filename);
        return NULL;
    }
    t_string url = form_url(uuid_filename);
    t_file *file_ent = mx_file_save_to_db((char *)filename, url);
    mx_strdel(&url);

    if (!file_ent) {
        mx_file_delete_uploaded(uuid_filename);
        mx_strdel(&uuid_filename);
        return NULL;
    }
    mx_strdel(&uuid_filename);

    return file_ent;
}

t_file *mx_file_service_delete(int file_id) {
    t_file *file = mx_file_delete_from_db(file_id);

    if (!file) {
        return NULL;
    }

    bool is_deleted = mx_file_delete_uploaded(file->name);

    if (!is_deleted) {
        mx_file_free(file);
        return NULL;
    }

    return file;
}
