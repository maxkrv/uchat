#include "shared.h"

int mx_extract_id_from_query(struct mg_str query, const char *key) {
    t_string value = mx_extract_value(query, key);

    if (!value) {
        return -1;
    }
    int id = mx_atoi(value);

    free(value);

    return id;
}

t_string mx_extract_value(struct mg_str from, const char *key) {
    return mx_decode_uri_component(mg_http_var(from, mg_str_s(key)));
}
