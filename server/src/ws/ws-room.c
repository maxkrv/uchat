#include "server.h"

static bool is_room_member(t_client *client, int room_id) {
    for (t_list *rooms = client->rooms; rooms; rooms = rooms->next) {
        t_room *room = rooms->data;

        if (room->id == room_id) {
            return true;
        }
    }

    return false;
}

void mx_ws_emit(const char *event, int room_id, cJSON *payload) {
    t_string m = mx_ws_message_stringify(event, payload);

    for (t_clients_list *clients = mx_ws_server_get()->clients; clients;
         clients = clients->next) {
        t_client *client = clients->data;

        if (!is_room_member(client, room_id)) {
            continue;
        }

        mg_ws_send(client->connection, m, mx_strlen(m), WEBSOCKET_OP_TEXT);
    }

    mx_strdel(&m);
}

void mx_ws_emit_everyone(const char *event, int room_id, cJSON *payload) {
    t_string m = mx_ws_message_stringify(event, payload);

    for (t_clients_list *clients = mx_ws_server_get()->clients; clients;
         clients = clients->next) {
        t_client *client = clients->data;

        mg_ws_send(client->connection, m, mx_strlen(m), WEBSOCKET_OP_TEXT);
    }

    mx_strdel(&m);
}

void mx_ws_emit_user(const char *event, int user_id, cJSON *payload) {
    t_client *client = mx_find_client(user_id);
    if (!client) {
        return;
    }
    t_string m = mx_ws_message_stringify(event, payload);

    mg_ws_send(client->connection, m, mx_strlen(m), WEBSOCKET_OP_TEXT);
    mx_strdel(&m);
}
