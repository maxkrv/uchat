#include "server.h"

t_pined_message *mx_room_pined_init(void) {
    t_pined_message *message = malloc(sizeof(t_pined_message));

    message->id = 0;
    message->message_id = 0;
    message->room_id = 0;
    message->created_at = 0;
    message->edited_at = 0;

    message->room = NULL;
    message->message = NULL;

    return message;
}

void mx_room_pined_free(t_pined_message *message) {
    if (!message) {
        return;
    }
    mx_room_free(message->room);
    mx_message_free(message->message);

    free(message);
}
