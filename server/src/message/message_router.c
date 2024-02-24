#include "message.h"

void mx_message_route(t_connection *conn, t_http_message *req) {
    t_crud_router router = {};

    mx_crud_route(&router, conn, req);
}
