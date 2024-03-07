#include "room.h"

void mx_room_route(t_connection *conn, t_http_message *req) {
    mx_route(conn, req, "/api/v1/rooms/find", MX_HTTP_METHOD_GET,
             mx_room_ctrl_get);

    mx_route(conn, req, "/api/v1/rooms", MX_HTTP_METHOD_PUT, mx_room_ctrl_put);
    mx_route(conn, req, "/api/v1/rooms", MX_HTTP_METHOD_DELETE,
             mx_room_ctrl_delete);

    mx_route(conn, req, "/api/v1/rooms/members/find", MX_HTTP_METHOD_GET,
             mx_room_ctrl_get_member);

    mx_route(conn, req, "/api/v1/rooms/members", MX_HTTP_METHOD_GET,
             mx_room_ctrl_get_members);
    mx_route(conn, req, "/api/v1/rooms/members", MX_HTTP_METHOD_POST,
             mx_room_ctrl_add_member);
    mx_route(conn, req, "/api/v1/rooms/members", MX_HTTP_METHOD_PUT,
             mx_room_ctrl_update_member);
    mx_route(conn, req, "/api/v1/rooms/members", MX_HTTP_METHOD_DELETE,
             mx_room_ctrl_delete_member);
    mx_route(conn, req, "/api/v1/rooms/pined", MX_HTTP_METHOD_GET,
             mx_room_ctrl_get_pined);
    mx_route(conn, req, "/api/v1/rooms/pined", MX_HTTP_METHOD_POST,
             mx_room_ctrl_pine_message);
    mx_route(conn, req, "/api/v1/rooms/pined", MX_HTTP_METHOD_DELETE,
             mx_room_ctrl_unpine);
}
