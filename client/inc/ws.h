#pragma once

#include "sdk/sdk.h"

typedef struct {
    t_string event;
    void *payload;
} t_ws_message;

void ws_client_init(const char *url);
void ws_client_free(void);

t_ws_message *mx_ws_message_init(void);
void mx_ws_message_free(t_ws_message *message, t_func_free payload_free);

t_ws_message *mx_ws_message_parse(t_string data, t_func_parser parse_payload);
void mx_ws_event_handler(struct mg_connection *conn, t_string event,
                         t_string data);
gboolean ws_reconect(gpointer user_data);
