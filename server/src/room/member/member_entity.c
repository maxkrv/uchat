#include "server.h"

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
