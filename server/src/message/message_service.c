#include "message.h"

t_message *mx_message_get(int id) {
    t_message *m = mx_message_repo_get(id);

    return m;
}

t_list *mx_message_get_many(int room_id) {
    return mx_message_repo_get_many(room_id);
}

t_message *mx_message_create(int user_id, t_message_create_dto *dto) {
    int message_id = mx_message_repo_create(user_id, dto);
    if (message_id <= 0) {
        return NULL;
    }
    for (t_list *l = dto->file_ids; l; l = l->next) {
        mx_message_file_repo_create(message_id, mx_atoi(l->data));
    }

    return mx_message_repo_get(message_id);
}

t_message *mx_message_put(int id, t_message_create_dto *dto) {
    bool ok = mx_message_repo_put(id, dto);
    if (!ok) {
        return NULL;
    }
    t_message *message = mx_message_get(id);

    for (t_list *l = message->files; l; l = l->next) {
        t_file *f = l->data;

        mx_message_file_repo_delete(f->id);
    }
    mx_message_free(message);

    for (t_list *l = dto->file_ids; l; l = l->next) {
        int file_id = mx_atoi(l->data);

        mx_message_file_repo_create(id, file_id);
    }
    return mx_message_repo_get(id);
}

t_message *mx_message_delete(int id) {
    t_message *message = mx_message_repo_get(id);
    bool ok = mx_message_repo_delete(id);

    if (!ok) {
        mx_message_free(message);
        return NULL;
    }

    return message;
}
