#include "sdk/sdk.h"

t_response *mx_sdk_user_find_by_id(int user_id) {
    t_sdk_env *env = mx_sdk_env_get();
    char *url = mg_mprintf("%s/api/v1/users/find?user_id=%d", env->backend_url,
                           user_id);
    t_response *response = mx_fetch(url, MX_HTTP_METHOD_GET,
                                    mx_headers_push_back_token(NULL), NULL);

    mx_parse_server_response(response, (t_func_parser)mx_user_parse_cjson);
    mx_strdel(&url);

    return response;
}

t_response *mx_sdk_user_find_by_tag(t_string tag) {
    t_sdk_env *env = mx_sdk_env_get();
    t_string encoded_tag = mx_encode_uri_component(mg_str(tag));
    char *url = mg_mprintf("%s/api/v1/users/find?tag=%s", env->backend_url,
                           encoded_tag);
    t_response *response = mx_fetch(url, MX_HTTP_METHOD_GET,
                                    mx_headers_push_back_token(NULL), NULL);

    mx_parse_server_response(response, (t_func_parser)mx_user_parse_cjson);
    mx_strdel(&url);
    mx_strdel(&encoded_tag);

    return response;
}

t_response *mx_sdk_user_find_by_name(t_string name) {
    t_sdk_env *env = mx_sdk_env_get();
    t_string encoded_name = mx_encode_uri_component(mg_str(name));
    char *url = mg_mprintf("%s/api/v1/users/find?name=%s", env->backend_url,
                           encoded_name);
    t_response *response = mx_fetch(url, MX_HTTP_METHOD_GET,
                                    mx_headers_push_back_token(NULL), NULL);

    mx_parse_server_response(response, (t_func_parser)mx_user_parse_cjson);
    mx_strdel(&url);
    mx_strdel(&encoded_name);

    return response;
}

t_response *mx_sdk_user_get_me(void) {
    t_sdk_env *env = mx_sdk_env_get();
    char *url = mg_mprintf("%s/api/v1/users/me", env->backend_url);
    t_response *response = mx_fetch(url, MX_HTTP_METHOD_GET,
                                    mx_headers_push_back_token(NULL), NULL);

    mx_parse_server_response(response, (t_func_parser)mx_user_parse_cjson);
    mx_strdel(&url);

    return response;
}

t_response *mx_sdk_user_put_me(t_user_update_dto *dto) {
    t_sdk_env *env = mx_sdk_env_get();
    char *url = mg_mprintf("%s/api/v1/users/me", env->backend_url);
    char *body = mx_user_update_dto_stringify(dto);
    t_response *response = mx_fetch(url, MX_HTTP_METHOD_PUT,
                                    mx_headers_push_back_token(NULL), body);

    mx_user_update_dto_free(dto);
    mx_parse_server_response(response, (t_func_parser)mx_user_parse_cjson);
    mx_strdel(&url);

    return response;
}
t_response *mx_sdk_user_delete_me(void) {
    t_sdk_env *env = mx_sdk_env_get();
    char *url = mg_mprintf("%s/api/v1/users/me", env->backend_url);
    t_response *response = mx_fetch(url, MX_HTTP_METHOD_DELETE,
                                    mx_headers_push_back_token(NULL), NULL);

    mx_parse_server_response(response, (t_func_parser)mx_user_parse_cjson);
    mx_strdel(&url);

    return response;
}

t_response *mx_sdk_user_favorites_get(void) {
    t_sdk_env *env = mx_sdk_env_get();
    char *url = mg_mprintf("%s/api/v1/users/me/favorites", env->backend_url);
    t_response *response = mx_fetch(url, MX_HTTP_METHOD_GET,
                                    mx_headers_push_back_token(NULL), NULL);

    mx_parse_array_server_response(response,
                                   (t_func_parser)mx_favorite_parse_cjson);
    mx_strdel(&url);

    return response;
}

t_response *mx_sdk_user_favorites_post(int room_id) {
    t_sdk_env *env = mx_sdk_env_get();
    char *url = mg_mprintf("%s/api/v1/users/me/favorites?room_id=%d",
                           env->backend_url, room_id);
    t_response *response =
        mx_fetch(url, MX_HTTP_METHOD_POST, mx_headers_push_back_token(NULL),
                 mx_strdup(MX_EMPTY));
    mx_parse_server_response(response, (t_func_parser)mx_favorite_parse_cjson);
    mx_strdel(&url);

    return response;
}

t_response *mx_sdk_user_favorites_delete(int room_id) {
    t_sdk_env *env = mx_sdk_env_get();
    char *url = mg_mprintf("%s/api/v1/users/me/favorites?room_id=%d",
                           env->backend_url, room_id);
    t_response *response = mx_fetch(url, MX_HTTP_METHOD_DELETE,
                                    mx_headers_push_back_token(NULL), NULL);

    mx_parse_server_response(response, (t_func_parser)mx_favorite_parse_cjson);
    mx_strdel(&url);

    return response;
}

t_response *mx_sdk_user_stickers_get(void) {
    t_sdk_env *env = mx_sdk_env_get();
    char *url = mg_mprintf("%s/api/v1/users/me/stickers", env->backend_url);
    t_response *response = mx_fetch(url, MX_HTTP_METHOD_GET,
                                    mx_headers_push_back_token(NULL), NULL);

    mx_parse_array_server_response(response,
                                   (t_func_parser)mx_file_parse_cjson);
    mx_strdel(&url);

    return response;
}

t_response *mx_sdk_user_stickers_post(int file_id) {
    t_sdk_env *env = mx_sdk_env_get();
    char *url = mg_mprintf("%s/api/v1/users/me/stickers?file_id=%d",
                           env->backend_url, file_id);
    t_response *response = mx_fetch(url, MX_HTTP_METHOD_POST,
                                    mx_headers_push_back_token(NULL), NULL);

    mx_parse_server_response(response, (t_func_parser)mx_file_parse_cjson);
    mx_strdel(&url);

    return response;
}
t_response *mx_sdk_user_stickers_delete(int favorite_id) {
    t_sdk_env *env = mx_sdk_env_get();
    char *url = mg_mprintf("%s/api/v1/users/me/stickers?file_id=%d",
                           env->backend_url, favorite_id);
    t_response *response = mx_fetch(url, MX_HTTP_METHOD_DELETE,
                                    mx_headers_push_back_token(NULL), NULL);

    mx_parse_server_response(response, (t_func_parser)mx_file_parse_cjson);
    mx_strdel(&url);

    return response;
}
