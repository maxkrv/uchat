#include "server.h"

t_room_pined_message *mx_init_room_pined(void) {
    t_room_pined_message *message = malloc(sizeof(t_room_pined_message));
    message->id = 0;
    message->message_id = 0;
    message->room_id = 0;
    message->created_at = 0;
    message->edited_at = 0;

    message->room = NULL;
    message->message = NULL;

    return message;
}

void mx_delete_room_pined(t_room_pined_message *message) {
    if (!message) {
        return;
    }
    mx_delete_room(message->room);
    mx_delete_message(message->message);

    free(message);
}
