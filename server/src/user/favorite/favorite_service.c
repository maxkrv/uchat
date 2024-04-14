#include "server.h"

t_list *mx_user_get_favorites(int user_id) {
    return mx_favorites_repo_get_many(user_id);
}

t_favorite_room *mx_user_get_favorite(int fav_id) {
    return mx_favorites_repo_get(fav_id);
}

t_favorite_room *mx_user_add_favorite(int user_id, int room_id) {
    if (!mx_is_user_member_of(room_id, user_id)) {
        return NULL;
    }
    int id = mx_favorites_repo_create(user_id, room_id);
    if (id <= 0) {
        return NULL;
    }
    return mx_user_get_favorite(id);
}

t_favorite_room *mx_user_delete_favorite(int id) {
    t_favorite_room *fav = mx_favorites_repo_get(id);

    if (!fav) {
        return NULL;
    }

    if (!mx_is_user_member_of(fav->room_id, fav->user_id)) {
        mx_favorite_room_free(fav);
        return NULL;
    }
    mx_favorites_repo_delete(id);

    return fav;
}
