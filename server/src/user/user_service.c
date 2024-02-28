#include "user.h"

t_user *mx_user_get(int id) {
    return mx_user_repo_get(id);
}

t_user *mx_user_create(t_user_create_dto *dto) {
    bool ok = mx_user_repo_create(dto);
    if (!ok) {
        return NULL;
    }

    return mx_user_repo_get(1);
}

t_user *mx_user_put(int id, t_user_create_dto *dto) {
    bool ok = mx_user_repo_put(id, dto);

    if (!ok) {
        return NULL;
    }

    return mx_user_repo_get(id);
}

t_user *mx_user_delete(int id) {
    t_user *user = mx_user_repo_get(id);
    bool ok = mx_user_repo_delete(id);

    if (!ok) {
        return NULL;
    }

    return user;
}
