#include "room.h"

void mx_room_route(t_connection *conn, t_http_message *req) {
    if (mx_route(conn, req, "/api/v1/rooms/find", MX_HTTP_METHOD_GET,
                 mx_room_ctrl_get)) {

    } else if (mx_route(conn, req, "/api/v1/rooms", MX_HTTP_METHOD_POST,
                        mx_room_ctrl_post)) {
    } else if (mx_route(conn, req, "/api/v1/rooms/direct", MX_HTTP_METHOD_POST,
                        mx_room_direct_ctrl_post)) {
    } else if (mx_route(conn, req, "/api/v1/rooms", MX_HTTP_METHOD_PUT,
                        mx_room_ctrl_put)) {
    } else if (mx_route(conn, req, "/api/v1/rooms", MX_HTTP_METHOD_DELETE,
                        mx_room_ctrl_delete)) {
    } else if (mx_route(conn, req, "/api/v1/rooms/members/find",
                        MX_HTTP_METHOD_GET, mx_room_ctrl_get_member)) {
    } else if (mx_route(conn, req, "/api/v1/rooms/members", MX_HTTP_METHOD_GET,
                        mx_room_ctrl_get_members)) {
    } else if (mx_route(conn, req, "/api/v1/rooms/members",
                        MX_HTTP_METHOD_POST, mx_room_ctrl_add_member)) {
    } else if (mx_route(conn, req, "/api/v1/rooms/members", MX_HTTP_METHOD_PUT,
                        mx_room_ctrl_update_member)) {
    } else if (mx_route(conn, req, "/api/v1/rooms/members",
                        MX_HTTP_METHOD_DELETE, mx_room_ctrl_delete_member)) {
    } else if (mx_route(conn, req, "/api/v1/rooms/pined", MX_HTTP_METHOD_GET,
                        mx_room_ctrl_get_pined)) {
    } else if (mx_route(conn, req, "/api/v1/rooms/pined", MX_HTTP_METHOD_POST,
                        mx_room_ctrl_pine_message)) {
    } else if (mx_route(conn, req, "/api/v1/rooms/pined",
                        MX_HTTP_METHOD_DELETE, mx_room_ctrl_unpine)) {
    } else {
        mx_http_reply_not_found(conn, req);
    }
}
