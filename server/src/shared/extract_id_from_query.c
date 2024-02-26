#include "shared.h"

int mx_extract_id_from_query(t_string query) {
    t_string value = mx_decode_uri_component(mg_http_var(query, mg_str_s("id")));

    if (!value.ptr) {
        return -1;
    }

    return mx_atoi(value.ptr);
}
