#include "server.h"

t_room *mx_room_init(void) {
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

void mx_room_free(t_room *room) {
    if (!room) {
        return;
    }

    mx_strdel(&room->name);
    mx_strdel(&room->description);
    mx_strdel(&room->type);

    mx_list_free(&room->messages, (t_func_free)mx_message_free);
    mx_list_free(&room->members, (t_func_free)mx_room_member_free);
    mx_list_free(&room->pined_messages, (t_func_free)mx_room_pined_free);

    mx_file_free(room->photo);

    free(room);
}
