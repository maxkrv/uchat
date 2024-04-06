#include "sdk/utils.h"

bool mx_is_response_ok(t_response *response) {
    return response->status_code >= 200 && response->status_code < 300;
}

bool mx_is_response_error(t_response *response) {
    return !mx_is_response_ok(response);
}
