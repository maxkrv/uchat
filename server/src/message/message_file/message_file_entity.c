#include "server.h"

t_message_file *mx_message_file_init(void) {
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

void mx_message_file_free(t_message_file *file) {
    if (!file) {
        return;
    }
    mx_message_free(file->message);
    mx_file_free(file->file);

    free(file);
}
