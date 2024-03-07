#include "user.h"
#include "auth.h"

t_user *mx_user_repo_get(int id) {
    t_user *user = mx_init_user();

    return user;
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
