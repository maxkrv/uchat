#include "server.h"

t_jwt_token mx_auth_login(t_login_dto *dto) {
    t_user *user = mx_user_repo_get_by_name(dto->name);

    if (!user) {
        return NULL;
    }

    if (!mx_comp_hash(dto->password, user->password_hash)) {

        mx_user_free(user);
        return NULL;
    }

    return mx_create_auth_jwt(user->id);
}

t_user *mx_auth_register(t_register_dto *dto) {
    t_user *user = mx_user_create(dto);

    if (!user) {
        return NULL;
    }

    t_room_create_dto *rc_dto = mx_room_create_dto_init();

    rc_dto->name = mx_strdup("Notes");
    rc_dto->description =
        mx_strdup("This is your private space. "
                  "Noone has access to this room except you. "
                  "Use it however you want.");
    rc_dto->type = mx_strdup(MX_ROOM_TYPE_NOTES);

    t_room *room = mx_room_create(rc_dto, user->id);

    mx_room_create_dto_free(rc_dto);
    mx_room_free(room);

    return user;
}

t_user *mx_auth_change_password(int id, t_change_password_dto *dto) {
    t_user *user = mx_user_repo_get(id);

    if (!user) {
        return NULL;
    }

    if (!mx_comp_hash(dto->old_password, user->password_hash)) {
        mx_user_free(user);
        return NULL;
    }

    return mx_user_change_password(id, dto->new_password);
}
