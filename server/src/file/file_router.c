#include "server.h"

void mx_file_route(t_connection *c, t_http_message *req) {

    if (mx_route(c, req, "/api/v1/files/upload", MX_HTTP_METHOD_POST,
                 mx_file_ctrl_upload)) {
    } else if (mx_route(c, req, "/api/v1/files/find", MX_HTTP_METHOD_GET,
                        mx_file_ctrl_get)) {
    } else {
        mx_http_reply_not_found(c, req);
    }
}
