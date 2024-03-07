#include "room.h"

void mx_room_route(t_connection *conn, t_http_message *req) {
    if (mg_match(req->uri, mg_str("/api/v1/rooms/find"), NULL)) {
        if (mx_is_method_equal(&req->method, MX_HTTP_METHOD_GET)) {
            mx_room_ctrl_get(conn, req);
            return;
        }
    } else if (mg_match(req->uri, mg_str("/api/v1/rooms"), NULL)) {
        if (mx_is_method_equal(&req->method, MX_HTTP_METHOD_PUT)) {
            mx_room_ctrl_put(conn, req);
            return;
        }
        if (mx_is_method_equal(&req->method, MX_HTTP_METHOD_DELETE)) {
            mx_room_ctrl_delete(conn, req);
            return;
        }
    } else if (mg_match(req->uri, mg_str("/api/v1/rooms/members/find"),
                        NULL)) {
        if (mx_is_method_equal(&req->method, MX_HTTP_METHOD_GET)) {
            mx_room_ctrl_get_member(conn, req);
            return;
        }
    } else if (mg_match(req->uri, mg_str("/api/v1/rooms/members"), NULL)) {
        if (mx_is_method_equal(&req->method, MX_HTTP_METHOD_GET)) {
            mx_room_ctrl_get_members(conn, req);
            return;
        }
        if (mx_is_method_equal(&req->method, MX_HTTP_METHOD_POST)) {
            mx_room_ctrl_add_member(conn, req);
            return;
        }
        if (mx_is_method_equal(&req->method, MX_HTTP_METHOD_PUT)) {
            mx_room_ctrl_update_member(conn, req);
            return;
        }
        if (mx_is_method_equal(&req->method, MX_HTTP_METHOD_DELETE)) {
            mx_room_ctrl_delete_member(conn, req);
            return;
        }
    } else if (mg_match(req->uri, mg_str("/api/v1/rooms/pined"), NULL)) {
        if (mx_is_method_equal(&req->method, MX_HTTP_METHOD_GET)) {
            mx_room_ctrl_get_pined(conn, req);
            return;
        }
        if (mx_is_method_equal(&req->method, MX_HTTP_METHOD_POST)) {
            mx_room_ctrl_pine_message(conn, req);
            return;
        }
        if (mx_is_method_equal(&req->method, MX_HTTP_METHOD_DELETE)) {
            mx_room_ctrl_unpine(conn, req);
            return;
        }
    }

    mx_http_reply_not_found(conn, req);
}
