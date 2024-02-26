#include "message.h"

void mx_message_ctrl_get(t_connection *c, t_http_message *m) {
    int message_id = mx_extract_id_from_query(m->query);

    if (message_id < 0) {
        mg_http_reply(c, HTTP_STATUS_UNPROCESSABLE_ENTITY, MX_EMPTY, MX_EMPTY);
    }

    t_message *message = mx_message_get(message_id);
    t_string json_string = mx_message_stringify(message);

    mg_http_reply(c, HTTP_STATUS_OK, MX_EMPTY, json_string.ptr);
}

void mx_message_ctrl_post(t_connection *c, t_http_message *m) {
}

void mx_message_ctrl_put(t_connection *c, t_http_message *m) {
}

void mx_message_ctrl_delete(t_connection *c, t_http_message *m) {
}
