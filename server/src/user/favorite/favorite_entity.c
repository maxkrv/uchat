#include "server.h"

t_favorite_room *mx_init_favorite_room(void) {
    t_favorite_room *room = malloc(sizeof(t_favorite_room));
    room->id = 0;
    room->room_id = 0;
    room->user_id = 0;
    room->created_at = 0;
    room->edited_at = 0;

    room->user = NULL;
    room->room = NULL;

    return room;
}

void mx_delete_favorite_room(t_favorite_room *room) {
    if (!room) {
        return;
    }
    mx_delete_user(room->user);
    mx_delete_room(room->room);

    free(room);
}
