#include "server.h"
static bool is_already_pined(t_list *list, int room_id) {
    for (t_list *fav = list; fav; fav = fav->next) {
        t_favorite_room *favorite = fav->data;

        if (favorite->room_id == room_id) {
            return true;
        }
    }

    return false;
}

t_list *mx_user_get_favorites(int user_id) {
    return mx_favorites_repo_get_many(user_id);
}

t_list *mx_user_add_favorite(int user_id, int room_id) {
    if (!mx_is_user_member_of(room_id, user_id)) {
        return NULL;
    }

    t_list *list = mx_favorites_repo_get_many(user_id);
    bool is_duplicate = is_already_pined(list, room_id);

    mx_list_free(&list, (t_func_void)mx_favorite_room_free);

    return is_duplicate ? NULL
                        : mx_room_get_pined_messages(
                              mx_favorites_repo_create(user_id, room_id));
}

t_list *mx_user_delete_favorite(int id) {
    t_favorite_room *fav = mx_favorites_repo_get(id);

    if (!fav) {
        return NULL;
    }

    if (!mx_is_user_member_of(fav->room_id, fav->user_id)) {
        mx_favorite_room_free(fav);
        return NULL;
    }
    mx_favorite_room_free(fav);

    return mx_room_get_pined_messages(fav->user_id);
}
