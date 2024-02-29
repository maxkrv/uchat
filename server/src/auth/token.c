#include "auth.h"

static char *extract_jwt_auth_token(t_http_message *req) {
    t_string *auth_header = mg_http_get_header(req, "Authorization");
    if (auth_header == NULL || strncmp(auth_header->ptr, "Bearer ", 7) != 0) {
        return NULL;
    }

    return mx_strndup(auth_header->ptr + 7, auth_header->len - 7);
}

int mx_user_id_from_auth_jwt(t_http_message *req) {
    char *token = extract_jwt_auth_token(req);
    if (!token) {
        return -1;
    }
    return mx_verify_auth_jwt(token);
}
