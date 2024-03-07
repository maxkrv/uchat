#include "server.h"

t_favorite_room *mx_favorites_repo_get(int id) {
    return NULL;
}
/* Returns [{discriptions + room + room.photo}] */
t_list *mx_favorites_repo_get_many(int user_id) {
    return NULL;
}

int mx_favorites_repo_create(int user_id, int room_id) {
    return 0;
}

bool mx_favorites_repo_delete(int id) {
    return true;
}
