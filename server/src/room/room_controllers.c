#include "room.h"

void mx_room_ctrl_get(t_connection *c, t_http_message *m) {
    // validate params
    int room_id = mx_extract_id_from_query(m->query);

    if (room_id < 0) {
        mg_http_reply(c, HTTP_STATUS_UNPROCESSABLE_ENTITY, MX_EMPTY, MX_EMPTY);
    }

    t_room *room = mx_room_get(room_id);
    t_string json_string = mx_room_stringify(room);

    mg_http_reply(c, HTTP_STATUS_OK, MX_EMPTY, json_string.ptr);
}

void mx_room_ctrl_post(t_connection *c, t_http_message *m) {
}

void mx_room_ctrl_put(t_connection *c, t_http_message *m) {
}

void mx_room_ctrl_delete(t_connection *c, t_http_message *m) {
}
