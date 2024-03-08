#include "server.h"

void mx_auth_route(t_connection *conn, t_http_message *req) {
    if (mx_route(conn, req, "/api/v1/auth/login", MX_HTTP_METHOD_POST,
                 mx_auth_ctrl_login)) {
    } else if (mx_route(conn, req, "/api/v1/auth/register",
                        MX_HTTP_METHOD_POST, mx_auth_ctrl_register)) {
    } else if (mx_route(conn, req, "/api/v1/auth/change/password",
                        MX_HTTP_METHOD_POST, mx_auth_ctrl_change_password)) {
    } else {
        mx_http_reply_not_found(conn, req);
    }
}
