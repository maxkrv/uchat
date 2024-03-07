#include "shared.h"
#include "http.h"

bool mx_is_method_equal(struct mg_str *m1, const char *m2) {
    return strncmp(m1->ptr, m2, m1->len) == 0;
}

void mx_route(t_connection *c, t_http_message *req, const char *path,
              const char *method, t_handler handler) {
    if (mg_match(req->uri, mg_str(path), NULL)) {
        if (mx_is_method_equal(&req->method, method)) {
            handler(c, req);
            return;
        }
    }
    mx_http_reply_not_found(c, req);
}
