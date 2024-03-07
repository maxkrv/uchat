#include "message.h"

void mx_message_route(t_connection *conn, t_http_message *req) {
    mx_route(conn, req, "/api/v1/messages/find", MX_HTTP_METHOD_GET,
             mx_message_ctrl_get);

    mx_route(conn, req, "/api/v1/messages", MX_HTTP_METHOD_GET,
             mx_message_ctrl_get_many);
    mx_route(conn, req, "/api/v1/messages", MX_HTTP_METHOD_POST,
             mx_message_ctrl_post);
    mx_route(conn, req, "/api/v1/messages", MX_HTTP_METHOD_PUT,
             mx_message_ctrl_put);
    mx_route(conn, req, "/api/v1/messages", MX_HTTP_METHOD_DELETE,
             mx_message_ctrl_delete);

    mx_route(conn, req, "/api/v1/messages/read", MX_HTTP_METHOD_GET,
             mx_message_ctrl_get_read);
    mx_route(conn, req, "/api/v1/messages/read/find", MX_HTTP_METHOD_GET,
             mx_message_ctrl_get_reads);
    mx_route(conn, req, "/api/v1/messages/read", MX_HTTP_METHOD_POST,
             mx_message_ctrl_read);
}
