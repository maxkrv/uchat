#include "sdk/sdk.h"

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

    room->is_favorite = false;

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
t_pined_message *mx_room_pined_init(void) {
    t_pined_message *message = malloc(sizeof(t_pined_message));
    message->id = 0;
    message->message_id = 0;
    message->room_id = 0;
    message->created_at = 0;
    message->edited_at = 0;

    message->room = NULL;
    message->message = NULL;

    return message;
}

void mx_room_pined_free(t_pined_message *message) {
    if (!message) {
        return;
    }
    mx_room_free(message->room);
    mx_message_free(message->message);

    free(message);
}

t_room_member *mx_room_member_init(void) {
    t_room_member *member = malloc(sizeof(t_room_member));
    member->id = 0;
    member->user_id = 0;
    member->room_id = 0;
    member->is_admin = 0;
    member->created_at = 0;
    member->edited_at = 0;

    member->room = NULL;
    member->user = NULL;

    return member;
}

void mx_room_member_free(t_room_member *member) {
    if (!member) {
        return;
    }
    mx_room_free(member->room);
    mx_user_free(member->user);

    free(member);
}
