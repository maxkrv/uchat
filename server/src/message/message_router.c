#include "message.h"

void mx_message_route(t_connection *conn, t_http_message *req) {
    t_crud_router router = {mx_message_ctrl_post, mx_message_ctrl_get,
                            mx_message_ctrl_put, mx_message_ctrl_delete};

    mx_crud_route(&router, conn, req);
}
