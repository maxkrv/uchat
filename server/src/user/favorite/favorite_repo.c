#include "server.h"

t_favorite_room *mx_favorites_repo_get(int id) {
    t_favorite_room *favorite = mx_favorite_room_init();

    return favorite;
}
/* Returns [{discriptions + room + room.photo}] */
t_list *mx_favorites_repo_get_many(int user_id) {
    t_list *list = NULL;

    mx_push_back(&list, mx_favorites_repo_get(1));

    return list;
}

int mx_favorites_repo_create(int user_id, int room_id) {
    return 1;
}

bool mx_favorites_repo_delete(int id) {
    return true;
}
