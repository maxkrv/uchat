#include "sdk/sdk.h"

void mx_parse_server_response(t_response *response, t_func_parser parser) {
    if (!response || !parser) {
        return;
    }

    if (mx_is_response_ok(response)) {
        response->data = mx_entity_parse_string(response->body, parser);
        return;
    }

    if (mx_is_response_error(response)) {
        response->exception = mx_server_exception_parse(response->body);
    }
}
void mx_parse_array_server_response(t_response *response,
                                    t_func_parser parser) {
    if (!response || !parser) {
        return;
    }

    if (response->status_code >= HTTP_STATUS_OK &&
        response->status_code < HTTP_STATUS_MULTIPLE_CHOICES) {
        response->data = mx_entities_parse_string(response->body, parser);
        return;
    }

    if (response->status_code >= HTTP_STATUS_BAD_REQUEST) {
        response->exception = mx_server_exception_parse(response->body);
    }
}
