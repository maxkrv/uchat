#include "sdk/utils.h"

t_string mx_decode_uri_component(struct mg_str str) {
    if (!str.ptr) {
        return NULL;
    }
    char buf[MX_BUFFER_SIZE];

    int size = mg_url_decode(str.ptr, str.len, buf, sizeof(buf), true);

    if (size == -1) {
        return NULL;
    }
    buf[size] = '\0';

    return mx_strndup(buf, size);
}

t_string mx_encode_uri_component(struct mg_str str) {
    if (!str.ptr) {
        return NULL;
    }
    char buf[MX_BUFFER_SIZE];

    int size = mg_url_encode(str.ptr, str.len, buf, sizeof(buf));

    if (size == -1) {
        return NULL;
    }
    buf[size] = '\0';

    return mx_strndup(buf, size);
}
