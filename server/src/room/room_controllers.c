#include "user.h"

void mx_room_ctrl_get(t_connection *c, t_http_message *m) {
    //validate params
    int room_id = mx_extract_id_from_query(m->query);

    if (room_id < 0) mg_http_reply(c, HTTP_STATUS_UNPROCESSABLE_ENTITY, "", "");

    t_room *room = mx_room_get(room_id);

    //user struct to json
    //send response
    t_string json_string = mx_room_stringify(room);

    mg_http_reply(c, HTTP_STATUS_OK, "", json_string.ptr);
    //send events to clients if necessary(for example "client new message created please update your data")
}

void mx_room_ctrl_post(t_connection *c, t_http_message *m) {}

void mx_room_ctrl_put(t_connection *c, t_http_message *m) {}

void mx_room_ctrl_delete(t_connection *c, t_http_message *m) {}

