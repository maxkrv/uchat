#include "server.h"

static void ws_upgrade(t_connection *conn, t_http_message *req) {
    t_user_id user_id = mx_auth(req);

    if (user_id <= 0) {
        mx_http_reply_exception(conn, req, HTTP_STATUS_UNAUTHORIZED,
                                "Invalid token provided");
        return;
    }
    MG_INFO(("## WS new client request ##"));
    MG_DEBUG(("## WS user id: %d ##", user_id));
    mg_ws_upgrade(conn, req, MX_OPTIONS_HEADERS);
}

void mx_http_msg_route(t_connection *conn, t_http_message *req) {
    if (mx_is_method_equal(&req->method, MX_HTTP_METHOD_OPTIONS)) {
        mg_http_reply(conn, HTTP_STATUS_OK, MX_OPTIONS_HEADERS, MX_EMPTY);
    } else if (mx_is_path_match(req, "/ws")) {
        ws_upgrade(conn, req);
    } else if (mx_is_path_match(req, "/assets/#")) {
        mg_http_serve_dir(conn, req, &mx_env_get()->static_dir_opt);
    } else if (mx_is_path_match(req, "/api/v1/users#")) {
        mx_user_route(conn, req);
    } else if (mx_is_path_match(req, "/api/v1/rooms#")) {
        mx_room_route(conn, req);
    } else if (mx_is_path_match(req, "/api/v1/messages#")) {
        mx_message_route(conn, req);
    } else if (mx_is_path_match(req, "/api/v1/auth#")) {
        mx_auth_route(conn, req);
    } else if (mx_is_path_match(req, "/api/v1/files#")) {
        mx_file_route(conn, req);
    } else {
        mx_http_reply_not_found(conn, req);
    }
}
