#include "server.h"

t_favorite_room *mx_favorite_room_init(void) {
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

void mx_favorite_room_free(t_favorite_room *room) {
    if (!room) {
        return;
    }
    mx_user_free(room->user);
    mx_room_free(room->room);

    free(room);
}
