#include "auth.h"

static char *extract_jwt_auth_token(t_http_message *req) {
    struct mg_str *auth_header = mg_http_get_header(req, "Authorization");

    if (auth_header == NULL || strncmp(auth_header->ptr, "Bearer ", 7) != 0) {
        return NULL;
    }

    return mx_strndup(auth_header->ptr + 7, auth_header->len - 7);
}

int mx_auth(t_http_message *req) {
    char *token = extract_jwt_auth_token(req);
    if (!token) {
        return -1;
    }
    t_user_id id = mx_verify_auth_jwt(token);

    mx_strdel(&token);

    return id;
}

t_string mx_token_stringify(t_jwt_token t) {
    cJSON *obj = cJSON_CreateObject();

    cJSON_AddStringToObject(obj, "token", t);

    t_string token = cJSON_PrintUnformatted(obj);

    cJSON_Delete(obj);

    return token;
}
