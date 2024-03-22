#include "server.h"

static t_ws_server *ws_server;

t_ws_server *mx_ws_server_init(void) {
    t_ws_server *ws_server = malloc(sizeof(t_ws_server));

    ws_server->clients = NULL;

    return ws_server;
}

t_ws_server *mx_ws_server_get(void) {
    return ws_server;
}

t_client *mx_create_client(t_connection *conne, t_user_id user_id,
                           t_list *rooms) {
    t_client *client = (t_client *)malloc(sizeof(t_client));
    client->connection = conne;
    client->user_id = user_id;
    client->rooms = rooms;
    client->created_at = time(NULL);

    return client;
}

void mx_delete_client(t_client *client) {
    mx_list_free(&client->rooms, (t_func_free)mx_room_free);

    free(client);
}

void mx_ws_server_free(t_ws_server *server) {
    mx_list_free((t_list **)&server->clients, (t_func_free)mx_delete_client);

    free(server);
}

void mx_ws_server_set(t_ws_server *server) {
    ws_server = server;
}
