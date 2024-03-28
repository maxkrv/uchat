#include "server.h"

void mx_user_route(t_connection *conn, t_http_message *req) {
    if (mx_route(conn, req, "/api/v1/users/find", MX_HTTP_METHOD_GET,
                 mx_user_ctrl_get)) {
    } else if (mx_route(conn, req, "/api/v1/users/me", MX_HTTP_METHOD_GET,
                        mx_user_ctrl_get_me)) {
    } else if (mx_route(conn, req, "/api/v1/users/me", MX_HTTP_METHOD_PUT,
                        mx_user_ctrl_put_me)) {
    } else if (mx_route(conn, req, "/api/v1/users/me", MX_HTTP_METHOD_DELETE,
                        mx_user_ctrl_delete_me)) {
    } else if (mx_route(conn, req, "/api/v1/users/me/rooms",
                        MX_HTTP_METHOD_GET, mx_user_ctrl_get_my_rooms)) {
    } else if (mx_route(conn, req, "/api/v1/users/me/favorites",
                        MX_HTTP_METHOD_GET, mx_user_ctrl_get_favorites)) {
    } else if (mx_route(conn, req, "/api/v1/users/me/favorites",
                        MX_HTTP_METHOD_POST, mx_user_ctrl_add_favorite)) {
    } else if (mx_route(conn, req, "/api/v1/users/me/favorites",
                        MX_HTTP_METHOD_DELETE, mx_user_ctrl_delete_favorite)) {
    } else if (mx_route(conn, req, "/api/v1/users/me/stickers",
                        MX_HTTP_METHOD_GET, mx_user_ctrl_get_stickers)) {
    } else if (mx_route(conn, req, "/api/v1/users/me/stickers",
                        MX_HTTP_METHOD_POST, mx_user_ctrl_post_sticker)) {
    } else if (mx_route(conn, req, "/api/v1/users/me/stickers",
                        MX_HTTP_METHOD_DELETE, mx_user_ctrl_delete_sticker)) {
    } else {
        mx_http_reply_not_found(conn, req);
    }
}
