#include "shared.h"

void mx_http_reply_exception(t_connection *conn, t_http_message *req,
                             int status_code, const char *message) {
    cJSON *obj = cJSON_CreateObject();
    char *method = mx_strndup(req->method.ptr, req->method.len);
    char *uri = mx_strndup(req->uri.ptr, req->uri.len);

    cJSON_AddNumberToObject(obj, "status", status_code);
    cJSON_AddNumberToObject(obj, "timestamp", time(NULL));
    cJSON_AddStringToObject(obj, "method", method);
    cJSON_AddStringToObject(obj, "uri", uri);
    cJSON_AddStringToObject(obj, "message", message);

    mg_http_reply(conn, status_code, MX_EMPTY, cJSON_PrintUnformatted(obj));

    mx_strdel(&uri);
    mx_strdel(&method);
}
