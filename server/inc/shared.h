#ifndef BACKEND_SHARED_H
#define BACKEND_SHARED_H

#include "base.h"
#include "utils.h"
#include "structs.h"

typedef void (*t_handler)(t_connection *c, t_http_message *m);

bool mx_route(t_connection *c, t_http_message *req, const char *path,
              const char *method, t_handler handler);
bool mx_is_method_equal(struct mg_str *m1, const char *m2);
bool mx_is_path_match(t_http_message *req, const char *path);

t_string mx_extract_value(struct mg_str from, const char *key);
int mx_extract_id_from_query(struct mg_str query, const char *key);
void mx_http_reply_exception(t_connection *conn, t_http_message *req,
                             int status_code, const char *message);
void mx_http_reply_not_found(t_connection *conn, t_http_message *req);
#endif
