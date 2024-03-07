#include "server.h"

t_file *mx_init_file(void) {
    t_file *file = malloc(sizeof(t_file));
    file->id = 0;
    file->url = NULL;
    file->name = NULL;
    file->created_at = 0;
    file->edited_at = 0;

    return file;
}

void mx_delete_file(t_file *file) {
    if (!file) {
        return;
    }

    mx_strdel(&file->url);
    mx_strdel(&file->name);

    free(file);
}
