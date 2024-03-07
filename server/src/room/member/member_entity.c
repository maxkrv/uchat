#include "server.h"

t_room_member *mx_init_room_member(void) {
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

void mx_delete_room_member(t_room_member *member) {
    if (!member) {
        return;
    }
    mx_delete_room(member->room);
    mx_delete_user(member->user);

    free(member);
}
