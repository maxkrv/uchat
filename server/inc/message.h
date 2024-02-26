#ifndef UCHAT_MESSAGE
#define UCHAT_MESSAGE

#include "base.h"
#include "utils.h"
#include "shared.h"

typedef struct {
    int room_id;
    int author_id;
    int reply_id;
    bool is_edited;
    char *text;
} t_message_create_dto;

typedef struct {
    int room_id;
    int author_id;
    int reply_id;
    bool is_edited;
    char *text;
} t_message_put_dto;

void mx_message_ctrl_get(t_connection *c, t_http_message *m);
void mx_message_ctrl_post(t_connection *c, t_http_message *m);
void mx_message_ctrl_put(t_connection *c, t_http_message *m);
void mx_message_ctrl_delete(t_connection *c, t_http_message *m);

t_message *mx_message_get(int id);
t_message *mx_message_create(t_message_create_dto *dto);
t_message *mx_message_put(int id, t_message_put_dto *dto);
t_message *mx_message_delete(int id);

cJSON *mx_message_to_cjson(t_message *message);
cJSON *mx_messages_to_cjson(t_list *messages);
t_string mx_message_stringify(t_message *message);

cJSON *mx_read_message_to_cjson(t_read_message *message);
cJSON *mx_read_messages_to_cjson(t_list *message);
t_string mx_read_message_stringify(t_read_message *message);

void mx_message_route(t_connection *conn, t_http_message *req);
#endif
