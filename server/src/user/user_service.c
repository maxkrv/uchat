#include "user.h"

t_user *mx_user_get(int id) {
    static t_user user = {.id = 0,
                          .name = "Me",
                          .tag = "@me",
                          .password_hash = "grgvwF)££(@wrasdfvme",
                          .photo_url = "url",
                          .status = "status",
                          .description = "description",
                          .created_at = 0,
                          .edited_at = 0,
                          .messages = NULL,
                          .rooms = NULL,
                          .read_messages = NULL};
    return &user;
}

t_user *mx_user_create(t_user_create_dto *dto) {
    return NULL;
}

t_user *mx_user_put(int id, t_user_create_dto *dto) {
    return NULL;
}

t_user *mx_user_delete(int id) {
    return NULL;
}
