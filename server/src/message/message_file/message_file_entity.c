#include "server.h"

t_message_file *mx_init_message_file(void) {
    t_message_file *member = malloc(sizeof(t_message_file));
    member->id = 0;
    member->message_id = 0;
    member->file_id = 0;
    member->created_at = 0;
    member->edited_at = 0;

    member->message = NULL;
    member->file = NULL;

    return member;
}

void mx_delete_message_file(t_message_file *file) {
    if (!file) {
        return;
    }
    mx_delete_message(file->message);
    mx_delete_file(file->file);

    free(file);
}
