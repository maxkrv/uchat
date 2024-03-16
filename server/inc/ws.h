#ifndef UCHAT_WS
#define UCHAT_WS

#include "base.h"

typedef struct {
    t_string event;
    void *payload;
} t_ws_message;

typedef struct {
    t_connection *connection;
    t_user_id user_id;
    t_list *rooms;
    int created_at;
} t_client;

typedef struct s_clients_list {
    t_client *data;
    struct s_clients_list *next;
} t_clients_list;

typedef struct {
    t_clients_list *clients;
} t_ws_server;

t_ws_server *mx_ws_server_init(void);
t_ws_server *mx_ws_server_get(void);
void mx_ws_server_free(t_ws_server *server);
void mx_ws_server_set(t_ws_server *server);

t_client *mx_create_client(t_connection *conne, t_user_id user_id,
                           t_list *rooms);
void mx_delete_client(t_client *client);

t_string mx_ws_message_stringify(const char *event, cJSON *payload);

void mx_ws_emit(const char *event, int room_id, cJSON *payload);
void mx_ws_emit_everyone(const char *event, int room_id, cJSON *payload);
void mx_ws_emit_user(const char *event, int user_id, cJSON *payload);

t_client *mx_find_client(int user_id);
void mx_delete_client_room(int user_id, int room_id);
void mx_delete_clients_room(int room_id);
void mx_push_client_room(int user_id, t_room *room);

#endif
