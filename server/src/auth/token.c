#include "auth.h"

static t_string extract_jwt_auth_token(t_http_message *req) {
    t_string *auth_header = mg_http_get_header(req, "Authorization");

    if (auth_header == NULL || strncmp(auth_header->ptr, "Bearer ", 7) != 0) {
        return mg_str_s(NULL);
    }

    return mg_str_n(auth_header->ptr + 7, auth_header->len - 7);
}

int mx_parse_jwt_auth_token(t_http_message *req) {
    t_string token = extract_jwt_auth_token(req);

    return 1;
}
