#include "message.h"

void mx_message_route(t_connection *conn, t_http_message *req) {
    if (mx_route(conn, req, "/api/v1/messages/find", MX_HTTP_METHOD_GET,
                 mx_message_ctrl_get)) {
    } else if (mx_route(conn, req, "/api/v1/messages", MX_HTTP_METHOD_GET,
                        mx_message_ctrl_get_many)) {
    } else if (mx_route(conn, req, "/api/v1/messages", MX_HTTP_METHOD_POST,
                        mx_message_ctrl_post)) {
    } else if (mx_route(conn, req, "/api/v1/messages", MX_HTTP_METHOD_PUT,
                        mx_message_ctrl_put)) {
    } else if (mx_route(conn, req, "/api/v1/messages", MX_HTTP_METHOD_DELETE,
                        mx_message_ctrl_delete)) {
    } else if (mx_route(conn, req, "/api/v1/messages/read", MX_HTTP_METHOD_GET,
                        mx_message_ctrl_get_read)) {
    } else if (mx_route(conn, req, "/api/v1/messages/read/find",
                        MX_HTTP_METHOD_GET, mx_message_ctrl_get_reads)) {
    } else if (mx_route(conn, req, "/api/v1/messages/read",
                        MX_HTTP_METHOD_POST, mx_message_ctrl_read)) {
    } else {
        mx_http_reply_not_found(conn, req);
    }
}
