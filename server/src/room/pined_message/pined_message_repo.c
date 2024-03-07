#include "server.h"

t_room_pined_message *mx_pined_repo_get(int id) {
    t_room_pined_message *m = mx_init_room_pined();

    return m;
}
/* Returns [{discriptions + room + room.photo}] */
t_list *mx_pined_repo_get_many(int room_id) {
    t_list *list;

    mx_push_back(&list, mx_pined_repo_get(1));

    return list;
}

int mx_pined_repo_create(int message_id, int room_id) {
    return 1;
}

bool mx_pined_repo_delete(int id) {
    return true;
}
