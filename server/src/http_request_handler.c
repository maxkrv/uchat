#include "server.h"

void mx_http_request_handler(t_connection *conn, int event, void *data) {
    if (event == MG_EV_READ) {
        if (conn->recv.len > MX_MAX_FILE_SIZE) {
            MG_ERROR(("Msg too large: %d", conn->recv.len));
            conn->is_draining = true; // close this connection
        }
    }
    if (event == MG_EV_HTTP_MSG) {
        t_http_message *req = data;

        if (mx_is_method_equal(&req->method, MX_HTTP_METHOD_OPTIONS)) {
            mg_http_reply(conn, HTTP_STATUS_OK, MX_OPTIONS_HEADERS, MX_EMPTY);
        } else if (mg_match(req->uri, mg_str("/api/v1/users/#"), NULL)) {
            mx_user_route(conn, req);
        } else if (mg_match(req->uri, mg_str("/api/v1/rooms/#"), NULL)) {
            mx_room_route(conn, req);
        } else if (mg_match(req->uri, mg_str("/api/v1/messages/#"), NULL)) {
            mx_message_route(conn, req);
        } else if (mg_match(req->uri, mg_str("/api/v1/auth/#"), NULL)) {
            mx_auth_route(conn, req);
        } else if (mg_match(req->uri, mg_str("/api/v1/files/#"), NULL)) {
            mx_file_route(conn, req);
        } else if (mg_match(req->uri, mg_str("/assets/#"), NULL)) {
            struct mg_http_serve_opts opt = {.root_dir =
                                                 mx_get_env()->static_dir_opt};

            mg_http_serve_dir(conn, req, &opt);
        } else {
            mx_http_reply_not_found(conn, req);
        }

        // Log request
        MG_INFO(("%.*s %.*s %lu -> %.*s %lu", req->method.len, req->method.ptr,
                 req->uri.len, req->uri.ptr, req->body.len, 3,
                 conn->send.buf + 9, conn->send.len));
    }
}
