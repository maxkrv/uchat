#include "user.h"
#include "auth.h"

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
                          .rooms = NULL};
    return &user;
}

t_user *mx_user_repo_get_by_tag(char *tag) {
    return mx_user_repo_get(1);
}

t_user *mx_user_repo_get_by_name(char *name) {
    return mx_user_repo_get(1);
}

t_user_id mx_user_repo_create(t_user_create_dto *dto) {
    return 1;
}

bool mx_user_repo_put(int id, t_user_update_dto *dto) {
    return true;
}

bool mx_user_repo_change_password(int id, char *password) {
    return true;
}

bool mx_user_repo_delete(int id) {
    return true;
}
