#include "sdk/utils.h"

t_header *mx_sdk_header_create(char *key, char *value) {
    t_header *h = (t_header *)malloc(sizeof(t_header));
    h->key = key;
    h->value = value;

    return h;
}

void mx_sdk_header_free(t_header *h) {
    if (!h) {
        return;
    }
    mx_strdel(&h->key);
    mx_strdel(&h->value);

    free(h);
}

t_list *mx_headers_push_back(t_list *headers, char *key, char *value) {
    if (!key || !value) {
        return headers;
    }
    t_header *header = mx_sdk_header_create(key, value);

    mx_push_back(&headers, header);

    return headers;
}

t_list *mx_headers_push_back_token(t_list *headers) {
    if (!mx_sdk_env_get()->jwt_token) {
        return NULL;
    }

    t_header *header = mx_sdk_header_create(
        mx_strdup("Authorization"),
        mg_mprintf("Bearer %s", mx_sdk_env_get()->jwt_token));

    mx_push_back(&headers, header);

    return headers;
}
