#include "server.h"

t_user *mx_init_user(void) {
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

void mx_delete_user(t_user *user) {
    if (!user) {
        return;
    }

    mx_strdel(&user->name);
    mx_strdel(&user->tag);
    mx_strdel(&user->password_hash);
    mx_strdel(&user->status);
    mx_strdel(&user->description);

    mx_delete_list(&user->messages, (t_func_void)mx_delete_message);
    mx_delete_list(&user->rooms, (t_func_void)mx_delete_room);
    mx_delete_list(&user->favorites, (t_func_void)mx_delete_favorite_room);

    mx_delete_file(user->photo);

    free(user);
}
