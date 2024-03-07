#include "shared.h"
#include "http.h"

bool mx_is_method_equal(struct mg_str *m1, char *m2) {
    return strncmp(m1->ptr, m2, m1->len) == 0;
}

void mx_crud_route(t_crud_router *router, t_connection *conn,
                   t_http_message *req) {
    struct mg_str method = req->method;

    if (mx_is_method_equal(&method, MX_HTTP_METHOD_GET)) {
        router->read(conn, req);
    } else if (mx_is_method_equal(&method, MX_HTTP_METHOD_POST)) {
        router->create(conn, req);
    } else if (mx_is_method_equal(&method, MX_HTTP_METHOD_PUT)) {
        router->update(conn, req);
    } else if (mx_is_method_equal(&method, MX_HTTP_METHOD_DELETE)) {
        router->delete (conn, req);
    }

    mx_http_reply_not_found(conn, req);
}
