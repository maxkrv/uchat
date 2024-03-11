#include "server.h"

/*
  User:
    descriptions(all exclude password);
    relations(photo;
 */
t_user *mx_user_get(int id) {
    t_user *user = mx_user_get_discription(id);

    if (!user) {
        return NULL;
    }
    user->password_hash = NULL;

    return user;
}

/*
  User:
    descriptions(all exclude password);
 */
t_user *mx_user_get_discription(int id) {
    t_user *user = mx_user_repo_get(id);

    if (!user) {
        return NULL;
    }
    user->password_hash = NULL;
    user->favorites = NULL;
    user->messages = NULL;
    user->rooms = NULL;

    return user;
}

t_user *mx_user_get_by_tag(char *tag) {
    t_user *user = mx_user_repo_get_by_tag(tag);

    if (!user) {
        return NULL;
    }

    user->password_hash = NULL;
    user->favorites = NULL;
    user->messages = NULL;
    user->rooms = NULL;

    return user;
}

t_list *mx_user_get_rooms(int user_id) {
    t_list *rooms = mx_members_repo_get_many_rooms(user_id);

    if (!rooms) {
        return NULL;
    }

    return rooms;
}

t_user *mx_user_create(t_user_create_dto *dto) {
    char *hashed = mx_hash_string(dto->password);

    mx_strdel(&dto->password);
    dto->password = hashed;

    t_user_id user_id = mx_user_repo_create(dto);

    if (user_id < 0) {
        return NULL;
    }

    return mx_user_get(user_id);
}

t_user *mx_user_put(int id, t_user_update_dto *dto) {
    bool ok = mx_user_repo_put(id, dto);

    if (!ok) {
        return NULL;
    }

    return mx_user_get(id);
}

t_user *mx_user_change_password(int id, char *password) {
    char *password_hash = mx_hash_string(password);

    if (!mx_user_repo_change_password(id, password)) {
        mx_strdel(&password_hash);
        return NULL;
    };
    mx_strdel(&password_hash);

    return mx_user_get(id);
}

t_user *mx_user_delete(int id) {
    t_user *user = mx_user_get(id);
    bool ok = mx_user_repo_delete(id);

    if (!user || !ok) {
        return NULL;
    }

    return user;
}
