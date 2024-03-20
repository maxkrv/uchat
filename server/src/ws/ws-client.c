#include "server.h"

t_client *mx_find_client(int user_id) {
    for (t_clients_list *clients = mx_ws_server_get()->clients; clients;
         clients = clients->next) {
        t_client *client = clients->data;

        if (client->user_id == user_id) {
            return client;
        }
    }

    return NULL;
}

void mx_delete_client_room(int user_id, int room_id) {
    t_client *client = mx_find_client(user_id);
    if (!client) {
        return;
    }

    for (t_list *list = client->rooms; list; list = list->next) {
        t_room *room = list->data;

        if (room->id == room_id) {
            mx_delete_node(&client->rooms, list, (t_func_free)mx_room_free);
            return;
        }
    }
}
void mx_delete_clients_room(int room_id) {
    for (t_clients_list *clients = mx_ws_server_get()->clients; clients;
         clients = clients->next) {
        t_client *client = clients->data;

        for (t_list *list = client->rooms; list; list = list->next) {
            t_room *room = list->data;

            if (room->id == room_id) {
                mx_delete_node(&client->rooms, list,
                               (t_func_free)mx_room_free);

            }
        }
    }
}

void mx_push_client_room(int user_id, t_room *room) {
    t_client *client = mx_find_client(user_id);
    if (!client) {
        return;
    }

    mx_push_back(&client->rooms, room);
}
