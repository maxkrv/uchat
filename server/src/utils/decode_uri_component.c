#include "utils.h"

t_string mx_decode_uri_component(t_string str) {
    char buf[MX_BUFFER_SIZE];

    int status = mg_url_decode(str.ptr, str.len, buf, sizeof(buf), true);

    if (status == -1) {
        return mg_str(NULL);
    }

    return mg_str(buf);
}
