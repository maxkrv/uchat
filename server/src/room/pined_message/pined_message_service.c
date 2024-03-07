#include "server.h"

static bool is_already_pined(t_list *list, int message_id) {
    for (t_list *fav = list; fav; fav = fav->next) {
        t_room_pined_message *pined = fav->data;

        if (pined->message_id == message_id) {
            return true;
        }
    }

    return false;
}

t_list *mx_room_get_pined_messages(int room_id) {
    return mx_pined_repo_get_many(room_id);
}

t_list *mx_room_pine_message(int message_id, int room_id) {
    t_list *list = mx_pined_repo_get_many(room_id);
    bool is_duplicate = is_already_pined(list, room_id);

    mx_delete_list(&list, (t_func_void)mx_delete_favorite_room);

    return is_duplicate ? NULL
                        : mx_room_get_pined_messages(
                              mx_pined_repo_create(message_id, room_id));
}

t_list *mx_room_unpine(int id) {
    t_room_pined_message *pin = mx_pined_repo_get(id);
    int room_id = pin->room_id;

    mx_pined_repo_delete(id);
    mx_delete_room_pined(pin);

    return mx_room_get_pined_messages(room_id);
}
