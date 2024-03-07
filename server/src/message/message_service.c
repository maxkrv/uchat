#include "message.h"

t_message *mx_message_get(int id) {
    t_message *m = mx_message_repo_get(id);

    m->author = mx_user_get_discription(m->author_id);
    m->message_files = NULL;
    m->readed_by = NULL;

    return m;
}

t_message *mx_message_create(t_message_create_dto *dto) {
    int id = mx_message_repo_create(dto);
    if (id <= 0) {
        return NULL;
    }

    return mx_message_repo_get(id);
}

t_message *mx_message_put(int id, t_message_create_dto *dto) {
    bool ok = mx_message_repo_put(id, dto);

    if (!ok) {
        return NULL;
    }

    return mx_message_repo_get(id);
}

t_message *mx_message_delete(int id) {
    t_message *user = mx_message_repo_get(id);
    bool ok = mx_message_repo_delete(id);

    if (!ok) {
        return NULL;
    }

    return user;
}
