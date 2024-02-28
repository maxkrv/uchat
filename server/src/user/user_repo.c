#include "user.h"

t_user *mx_user_repo_get(int id) {
    static t_user user = {.id = 0,
                          .name = "Me",
                          .tag = "@me",
                          .password_hash = "grgvwF)££(@wrasdfvme",
                          .photo_id = 1,
                          .status = "status",
                          .description = "description",
                          .created_at = 0,
                          .edited_at = 0,
                          .messages = NULL,
                          .rooms_member = NULL,
                          .read_messages = NULL};
    return &user;
}

bool mx_user_repo_create(t_user_create_dto *dto) {
    return true;
}

bool mx_user_repo_put(int id, t_user_create_dto *dto) {
    return true;
}

bool mx_user_repo_delete(int id) {
    return true;
}
