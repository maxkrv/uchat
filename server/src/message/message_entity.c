#include "server.h"

t_message *mx_message_init(void) {
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

void mx_message_free(t_message *message) {
    if (!message) {
        return;
    }

    mx_strdel(&message->text);

    mx_user_free(message->author);
    mx_room_free(message->room);
    mx_message_free(message->reply);

    mx_list_free(&message->readed_by, (t_func_free)mx_read_message_free);
    mx_list_free(&message->files, (t_func_free)mx_file_free);

    free(message);
}
