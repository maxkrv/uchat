#include "server.h"

void mx_user_route(t_connection *conn, t_http_message *req) {
    mx_route(conn, req, "/api/v1/users/find", MX_HTTP_METHOD_GET,
             mx_user_ctrl_get);

    mx_route(conn, req, "/api/v1/users/me", MX_HTTP_METHOD_GET,
             mx_user_ctrl_get_me);
    mx_route(conn, req, "/api/v1/users/me", MX_HTTP_METHOD_PUT,
             mx_user_ctrl_put_me);
    mx_route(conn, req, "/api/v1/users/me", MX_HTTP_METHOD_DELETE,
             mx_user_ctrl_delete_me);

    mx_route(conn, req, "/api/v1/users/me/rooms", MX_HTTP_METHOD_GET,
             mx_user_ctrl_get_my_rooms);

    mx_route(conn, req, "/api/v1/users/me/favorites", MX_HTTP_METHOD_GET,
             mx_user_ctrl_get_favorites);
    mx_route(conn, req, "/api/v1/users/me/favorites", MX_HTTP_METHOD_POST,
             mx_user_ctrl_add_favorite);
    mx_route(conn, req, "/api/v1/users/me/favorites", MX_HTTP_METHOD_DELETE,
             mx_user_ctrl_delete_favorite);
}
