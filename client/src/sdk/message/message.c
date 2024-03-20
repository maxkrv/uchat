#include "sdk/sdk.h"

t_response *mx_sdk_message_find(int message_id) {
    t_sdk_env *env = mx_sdk_env_get();
    char *url = mg_mprintf("%s/api/v1/messages/find?message_id=%d",
                           env->backend_url, message_id);
    t_response *response = mx_fetch(url, MX_HTTP_METHOD_GET,
                                    mx_headers_push_back_token(NULL), NULL);

    mx_parse_server_response(response, (t_func_parser)mx_message_parse_cjson);
    mx_strdel(&url);

    return response;
}
t_response *mx_sdk_messages_get(int room_id) {
    t_sdk_env *env = mx_sdk_env_get();
    char *url =
        mg_mprintf("%s/api/v1/messages?room_id=%d", env->backend_url, room_id);
    t_response *response = mx_fetch(url, MX_HTTP_METHOD_GET,
                                    mx_headers_push_back_token(NULL), NULL);

    mx_parse_array_server_response(response,
                                   (t_func_parser)mx_message_parse_cjson);
    mx_strdel(&url);

    return response;
}
t_response *mx_sdk_message_post(t_message_create_dto *message) {
    t_sdk_env *env = mx_sdk_env_get();
    char *url = mg_mprintf("%s/api/v1/messages", env->backend_url);
    char *body = mx_message_create_dto_stringify(message);
    t_response *response = mx_fetch(url, MX_HTTP_METHOD_POST,
                                    mx_headers_push_back_token(NULL), body);

    mx_message_create_dto_free(message);
    mx_parse_server_response(response, (t_func_parser)mx_message_parse_cjson);
    mx_strdel(&url);

    return response;
}
t_response *mx_sdk_message_put(int message_id, t_message_create_dto *message) {
    t_sdk_env *env = mx_sdk_env_get();
    char *url = mg_mprintf("%s/api/v1/messages?message_id=%d",
                           env->backend_url, message_id);
    char *body = mx_message_create_dto_stringify(message);
    t_response *response = mx_fetch(url, MX_HTTP_METHOD_PUT,
                                    mx_headers_push_back_token(NULL), body);

    mx_message_create_dto_free(message);
    mx_parse_server_response(response, (t_func_parser)mx_message_parse_cjson);
    mx_strdel(&url);

    return response;
}
t_response *mx_sdk_message_delete(int message_id) {
    t_sdk_env *env = mx_sdk_env_get();
    char *url = mg_mprintf("%s/api/v1/messages?message_id=%d",
                           env->backend_url, message_id);
    t_response *response = mx_fetch(url, MX_HTTP_METHOD_DELETE,
                                    mx_headers_push_back_token(NULL), NULL);

    mx_parse_server_response(response, (t_func_parser)mx_message_parse_cjson);
    mx_strdel(&url);

    return response;
}
t_response *mx_sdk_readers_get(int message_id) {
    t_sdk_env *env = mx_sdk_env_get();
    char *url = mg_mprintf("%s/api/v1/messages/readers?message_id=%d",
                           env->backend_url, message_id);
    t_response *response = mx_fetch(url, MX_HTTP_METHOD_GET,
                                    mx_headers_push_back_token(NULL), NULL);

    mx_parse_array_server_response(response,
                                   (t_func_parser)mx_reader_parse_cjson);
    mx_strdel(&url);

    return response;
}

t_response *mx_sdk_readers_post(int message_id) {
    t_sdk_env *env = mx_sdk_env_get();
    char *url = mg_mprintf("%s/api/v1/messages/readers?message_id=%d",
                           env->backend_url, message_id);
    t_response *response = mx_fetch(url, MX_HTTP_METHOD_POST,
                                    mx_headers_push_back_token(NULL), NULL);

    mx_parse_server_response(response, (t_func_parser)mx_reader_parse_cjson);
    mx_strdel(&url);

    return response;
}
t_response *mx_sdk_reader_find(int read_id) {
    t_sdk_env *env = mx_sdk_env_get();
    char *url = mg_mprintf("%s/api/v1/messages/readers/find?read_id=%d",
                           env->backend_url, read_id);
    t_response *response = mx_fetch(url, MX_HTTP_METHOD_GET,
                                    mx_headers_push_back_token(NULL), NULL);

    mx_parse_server_response(response, (t_func_parser)mx_reader_parse_cjson);
    mx_strdel(&url);

    return response;
}
