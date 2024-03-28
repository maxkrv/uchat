#include "server.h"

t_file *mx_sticker_delete(int user_id, int file_id) {
    t_file *file = mx_sticker_get(user_id, file_id);

    if (!file) {
        return NULL;
    }

    if (!mx_sticker_repo_delete(user_id, file_id)) {
        mx_file_free(file);
        return NULL;
    };

    return file;
}

t_file *mx_sticker_add(int user_id, int file_id) {
    if (mx_sticker_repo_create(user_id, file_id) <= 0) {
        return NULL;
    }

    return mx_sticker_get(user_id, file_id);
}

t_list *mx_stickers_get(int user_id) {
    return mx_sticker_repo_get_many(user_id);
}

t_file *mx_sticker_get(int user_id, int file_id) {
    return mx_sticker_repo_get(user_id, file_id);
}
