#include "sdk/sdk.h"

t_server_exception *mx_server_exception_init(void) {
    t_server_exception *e =
        (t_server_exception *)malloc(sizeof(t_server_exception));
    e->status = 0;
    e->message = NULL;
    e->timestamp = 0;
    e->method = NULL;
    e->uri = NULL;

    return e;
}

t_server_exception *mx_server_exception_parse(t_string str) {
    cJSON *json = cJSON_Parse(str);

    if (!json) {
        return NULL;
    }
    t_server_exception *e = mx_server_exception_init();

    e->status = mx_cjson_get_number(json, "status");
    e->timestamp = mx_cjson_get_number(json, "timestamp");
    e->message = mx_cjson_get_string(json, "message");
    e->method = mx_cjson_get_string(json, "method");
    e->uri = mx_cjson_get_string(json, "uri");

    return e;
}

void mx_server_exception_free(t_server_exception *e) {
    if (!e) {
        return;
    }

    mx_strdel(&e->message);
    mx_strdel(&e->uri);
    mx_strdel(&e->method);

    free(e);
}
