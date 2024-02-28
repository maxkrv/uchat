#include "server.h"

void mx_http_request_handler(t_connection *conn, int event, void *data) {
    if (event == MG_EV_HTTP_MSG) {
        t_http_message *req = data;

        if (mg_match(req->uri, mg_str("/api/v1/users"), NULL)) {
            mx_user_route(conn, req);
        } else if (mg_match(req->uri, mg_str("/api/v1/rooms"), NULL)) {
            mx_room_route(conn, req);
        } else if (mg_match(req->uri, mg_str("/api/v1/messages"), NULL)) {
            mx_message_route(conn, req);
        } else {
            MG_INFO(("Common not found"));
            mg_http_reply(conn, HTTP_STATUS_NOT_FOUND, MX_EMPTY, MX_EMPTY);
        }

        // Log request
        MG_INFO(("%.*s %.*s %lu -> %.*s %lu", req->method.len, req->method.ptr,
                 req->uri.len, req->uri.ptr, req->body.len, 3,
                 conn->send.buf + 9, conn->send.len));
    }
}
