#include "user.h"

void mx_user_ctrl_get(t_connection *c, t_http_message *m) {
    // validate params
    int user_id = mx_extract_id_from_query(m->query);

    if (user_id < 0) {
        mg_http_reply(c, HTTP_STATUS_UNPROCESSABLE_ENTITY, MX_EMPTY, MX_EMPTY);
    }

    t_user *user = mx_user_get(user_id);

    // user struct to json
    // send response
    t_string json_string = mx_user_stringify(user);

    mg_http_reply(c, HTTP_STATUS_OK, MX_EMPTY, json_string.ptr);
    // send events to clients if necessary(for example "client new message
    // created please update your data")
}

void mx_user_ctrl_post(t_connection *c, t_http_message *m) {
}

void mx_user_ctrl_put(t_connection *c, t_http_message *m) {
}

void mx_user_ctrl_delete(t_connection *c, t_http_message *m) {
}
