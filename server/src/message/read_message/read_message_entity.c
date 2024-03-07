#include "server.h"

t_read_message *mx_init_read_message(void) {
    t_read_message *member = malloc(sizeof(t_read_message));
    member->id = 0;
    member->user_id = 0;
    member->message_id = 0;
    member->created_at = 0;
    member->edited_at = 0;

    member->message = NULL;
    member->user = NULL;

    return member;
}

void mx_delete_read_message(t_read_message *member) {
    if (!member) {
        return;
    }
    mx_delete_message(member->message);
    mx_delete_user(member->user);

    free(member);
}
