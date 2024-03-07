#include "server.h"

void mx_auth_route(t_connection *conn, t_http_message *req) {
    if (mg_match(req->uri, mg_str("/api/v1/auth/login"), NULL)) {
        if (mx_is_method_equal(&req->method, MX_HTTP_METHOD_POST)) {
            mx_auth_ctrl_login(conn, req);
            return;
        }
    } else if (mg_match(req->uri, mg_str("/api/v1/auth/register"), NULL)) {
        if (mx_is_method_equal(&req->method, MX_HTTP_METHOD_POST)) {
            mx_auth_ctrl_register(conn, req);
            return;
        }
    } else if (mg_match(req->uri, mg_str("/api/v1/auth/change/password"),
                        NULL)) {
        if (mx_is_method_equal(&req->method, MX_HTTP_METHOD_POST)) {
            mx_auth_ctrl_change_password(conn, req);
            return;
        }
    }

    mx_http_reply_not_found(conn, req);
}
