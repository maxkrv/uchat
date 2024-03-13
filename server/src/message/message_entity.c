#include "server.h"

t_message *mx_init_message(void) {
    t_message *message = malloc(sizeof(t_message));
    message->id = 0;
    message->room_id = 0;
    message->author_id = 0;
    message->reply_id = 0;
    message->text = NULL;

    message->created_at = 0;
    message->edited_at = 0;

    message->readed_by = NULL;
    message->author = NULL;
    message->room = NULL;
    message->reply = NULL;
    message->files = NULL;

    return message;
}

void mx_delete_message(t_message *message) {
    if (!message) {
        return;
    }

    mx_strdel(&message->text);

    mx_delete_user(message->author);
    mx_delete_room(message->room);
    mx_delete_message(message->reply);

    mx_delete_list(&message->readed_by, (t_func_void)mx_delete_read_message);
    mx_delete_list(&message->files, (t_func_void)mx_delete_file);

    free(message);
}
