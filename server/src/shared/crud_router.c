#include "shared.h"
#include "http.h"

static bool is_method_equal(t_string *m1, char *m2) {
    return strncmp(m1->ptr, m2, m1->len) == 0;
}

void mx_crud_route(t_crud_router *router, t_connection *conn,
                   t_http_message *req) {
    t_string method = req->method;

    if (is_method_equal(&method, MX_HTTP_METHOD_GET)) {
        router->read(conn, req);
    } else if (is_method_equal(&method, MX_HTTP_METHOD_POST)) {
        router->create(conn, req);
    } else if (is_method_equal(&method, MX_HTTP_METHOD_PUT)) {
        router->update(conn, req);
    } else if (is_method_equal(&method, MX_HTTP_METHOD_DELETE)) {
        router->delete (conn, req);
    }
}
