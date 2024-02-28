#include "message.h"

t_message *mx_message_get(int id) {
    return mx_message_repo_get(id);
}

t_message *mx_message_create(t_message_create_dto *dto) {
    bool ok = mx_message_repo_create(dto);
    if (!ok) {
        return NULL;
    }

    return mx_message_repo_get(1);
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
