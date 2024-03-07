#include "server.h"

t_room *mx_init_room(void) {
    t_room *room = malloc(sizeof(t_room));
    room->id = 0;
    room->name = NULL;
    room->type = NULL;
    room->photo_id = 0;
    room->description = NULL;
    room->created_at = 0;
    room->edited_at = 0;

    room->messages = NULL;
    room->members = NULL;
    room->pined_messages = NULL;
    room->photo = NULL;

    return room;
}

void mx_delete_room(t_room *room) {
    if (!room) {
        return;
    }

    mx_strdel(&room->name);
    mx_strdel(&room->description);
    mx_strdel(&room->type);

    mx_delete_list(&room->messages, (t_func_void)mx_delete_message);
    mx_delete_list(&room->members, (t_func_void)mx_delete_room_member);
    mx_delete_list(&room->pined_messages, (t_func_void)mx_delete_room_pined);

    mx_delete_file(room->photo);

    free(room);
}
