#include "server.h"

void mx_user_route(t_connection *conn, t_http_message *req) {
    if (mg_match(req->uri, mg_str("/api/v1/users/find"), NULL)) {
        if (mx_is_method_equal(&req->method, MX_HTTP_METHOD_GET)) {
            mx_user_ctrl_get(conn, req);
            return;
        }
    } else if (mg_match(req->uri, mg_str("/api/v1/users/me"), NULL)) {
        if (mx_is_method_equal(&req->method, MX_HTTP_METHOD_GET)) {
            mx_user_ctrl_get_me(conn, req);
            return;
        }
        if (mx_is_method_equal(&req->method, MX_HTTP_METHOD_PUT)) {
            mx_user_ctrl_put_me(conn, req);
            return;
        }
        if (mx_is_method_equal(&req->method, MX_HTTP_METHOD_DELETE)) {
            mx_user_ctrl_delete_me(conn, req);
            return;
        }
    } else if (mg_match(req->uri, mg_str("/api/v1/users/me/rooms"), NULL)) {
        if (mx_is_method_equal(&req->method, MX_HTTP_METHOD_GET)) {
            mx_user_ctrl_get_my_rooms(conn, req);
            return;
        }
    } else if (mg_match(req->uri, mg_str("/api/v1/users/me/favorites"),
                        NULL)) {
        if (mx_is_method_equal(&req->method, MX_HTTP_METHOD_GET)) {
            mx_user_ctrl_get_favorites(conn, req);
            return;
        }
        if (mx_is_method_equal(&req->method, MX_HTTP_METHOD_POST)) {
            mx_user_ctrl_add_favorite(conn, req);
            return;
        }
        if (mx_is_method_equal(&req->method, MX_HTTP_METHOD_DELETE)) {
            mx_user_ctrl_delete_favorite(conn, req);
            return;
        }
    }

    mx_http_reply_not_found(conn, req);
}
