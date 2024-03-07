#include "user.h"
#include "auth.h"

t_jwt_token mx_auth_login(t_login_dto *dto) {
    t_user *user = mx_user_repo_get_by_name(dto->name);

    if (!user) {
        return NULL;
    }
    if (!mx_comp_hash(dto->password, user->password_hash)) {
        mx_delete_user(user);
        return NULL;
    }

    return mx_create_auth_jwt(user->id);
}

t_user *mx_auth_register(t_register_dto *dto) {
    return mx_user_create(dto);
}

t_user *mx_auth_change_password(int id, t_change_password_dto *dto) {
    t_user *user = mx_user_repo_get(id);

    if (!user) {
        return NULL;
    }

    if (!mx_comp_hash(dto->old_password, user->password_hash)) {
        mx_delete_user(user);
        return NULL;
    }

    return mx_user_change_password(id, dto->new_password);
}
