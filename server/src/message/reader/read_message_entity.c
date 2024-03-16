#include "server.h"

t_read_message *mx_read_message_init(void) {
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

void mx_read_message_free(t_read_message *member) {
    if (!member) {
        return;
    }
    mx_message_free(member->message);
    mx_user_free(member->user);

    free(member);
}
