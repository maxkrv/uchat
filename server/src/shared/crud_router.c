#include "shared.h"
#include "http.h"

void mx_crud_route(t_crud_router *router, t_connection *conn, t_http_message *req) {
    struct mg_str method = req->method;

    if (mg_vcmp(&method, MX_HTTP_METHOD_GET)) {
        router->read(conn, req);
    } else if (mg_vcmp(&method, MX_HTTP_METHOD_POST)) {
        router->create(conn, req);
    } else if (mg_vcmp(&method, MX_HTTP_METHOD_PUT)) {
        router->update(conn, req);
    } else if (mg_vcmp(&method, MX_HTTP_METHOD_DELETE)) {
        router->delete(conn, req);
    }
}
