#include "server.h"

t_list *mx_room_get_pined_messages(int room_id) {
    return mx_pined_repo_get_many(room_id);
}

t_pined_message *mx_room_pine_message(int message_id, int room_id) {
    int pined_id = mx_pined_repo_create(message_id, room_id);

    if (pined_id <= 0) {
        return NULL;
    }

    return mx_pined_repo_get(pined_id);
}

t_pined_message *mx_room_unpine(int id) {
    t_pined_message *pin = mx_pined_repo_get(id);
    if (!pin) {
        return NULL;
    }
    if (!mx_pined_repo_delete(id)) {
        mx_room_pined_free(pin);
        return NULL;
    };

    return pin;
}
