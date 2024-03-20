#include "sdk/sdk.h"

t_user *mx_user_init(void) {
    t_user *user = malloc(sizeof(t_user));
    user->id = 0;
    user->name = NULL;

    user->tag = NULL;
    user->password_hash = NULL;
    user->photo_id = 0;
    user->status = NULL;
    user->description = NULL;
    user->created_at = 0;
    user->edited_at = 0;

    user->messages = NULL;
    user->rooms = NULL;
    user->favorites = NULL;
    user->photo = NULL;

    return user;
}

void mx_user_free(t_user *user) {
    if (!user) {
        return;
    }

    mx_strdel(&user->name);
    mx_strdel(&user->tag);
    mx_strdel(&user->password_hash);
    mx_strdel(&user->status);
    mx_strdel(&user->description);

    mx_list_free(&user->messages, (t_func_free)mx_message_free);
    mx_list_free(&user->rooms, (t_func_free)mx_room_free);
    mx_list_free(&user->favorites, (t_func_free)mx_favorite_room_free);

    mx_file_free(user->photo);

    free(user);
}

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
