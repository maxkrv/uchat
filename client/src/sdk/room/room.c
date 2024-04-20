#include "sdk/sdk.h"

t_response *mx_sdk_room_find(int room_id) {
    t_sdk_env *env = mx_sdk_env_get();
    char *url = mg_mprintf("%s/api/v1/rooms/find?room_id=%d", env->backend_url,
                           room_id);
    t_response *response = mx_fetch(url, MX_HTTP_METHOD_GET, NULL, NULL);

    mx_parse_server_response(response, (t_func_parser)mx_room_parse_cjson);
    mx_strdel(&url);

    return response;
}
t_response *mx_sdk_rooms_get(void) {
    t_sdk_env *env = mx_sdk_env_get();
    char *url = mg_mprintf("%s/api/v1/users/me/rooms", env->backend_url);
    t_response *response = mx_fetch(url, MX_HTTP_METHOD_GET,
                                    mx_headers_push_back_token(NULL), NULL);

    mx_parse_array_server_response(response,
                                   (t_func_parser)mx_room_parse_cjson);
    mx_strdel(&url);

    return response;
}
t_response *mx_sdk_room_post(t_room_create_dto *room) {
    t_sdk_env *env = mx_sdk_env_get();
    char *url = mg_mprintf("%s/api/v1/rooms", env->backend_url);
    char *body = mx_room_create_dto_stringify(room);
    t_response *response = mx_fetch(url, MX_HTTP_METHOD_POST,
                                    mx_headers_push_back_token(NULL), body);

    mx_room_create_dto_free(room);
    mx_parse_server_response(response, (t_func_parser)mx_room_parse_cjson);
    mx_strdel(&url);

    return response;
}
t_response *mx_sdk_room_direct_post(t_room_create_dto *room,
                                    t_string username) {
    t_sdk_env *env = mx_sdk_env_get();
    t_string encoded_username = mx_encode_uri_component(mg_str(username));
    char *url = mg_mprintf("%s/api/v1/rooms/direct?username=%s",
                           env->backend_url, encoded_username);
    char *body = mx_room_create_dto_stringify(room);
    t_response *response = mx_fetch(url, MX_HTTP_METHOD_POST,
                                    mx_headers_push_back_token(NULL), body);

    mx_room_create_dto_free(room);
    mx_parse_server_response(response, (t_func_parser)mx_room_parse_cjson);
    mx_strdel(&url);
    mx_strdel(&encoded_username);

    return response;
}
t_response *mx_sdk_room_put(int room_id, t_room_create_dto *room) {
    t_sdk_env *env = mx_sdk_env_get();
    char *url =
        mg_mprintf("%s/api/v1/rooms?room_id=%d", env->backend_url, room_id);
    char *body = mx_room_create_dto_stringify(room);
    t_response *response = mx_fetch(url, MX_HTTP_METHOD_PUT,
                                    mx_headers_push_back_token(NULL), body);

    mx_room_create_dto_free(room);
    mx_parse_server_response(response, (t_func_parser)mx_room_parse_cjson);
    mx_strdel(&url);

    return response;
}
t_response *mx_sdk_room_delete(int room_id) {
    t_sdk_env *env = mx_sdk_env_get();
    char *url =
        mg_mprintf("%s/api/v1/rooms?room_id=%d", env->backend_url, room_id);
    t_response *response = mx_fetch(url, MX_HTTP_METHOD_DELETE,
                                    mx_headers_push_back_token(NULL), NULL);

    mx_parse_server_response(response, (t_func_parser)mx_room_parse_cjson);
    mx_strdel(&url);

    return response;
}

t_response *mx_sdk_room_members_find(int member_id) {
    t_sdk_env *env = mx_sdk_env_get();
    char *url = mg_mprintf("%s/api/v1/rooms/members/find?member_id=%d",
                           env->backend_url, member_id);
    t_response *response = mx_fetch(url, MX_HTTP_METHOD_GET, NULL, NULL);

    mx_parse_server_response(response, (t_func_parser)mx_member_parse_cjson);
    mx_strdel(&url);

    return response;
}
t_response *mx_sdk_room_members_get(int room_id) {
    t_sdk_env *env = mx_sdk_env_get();
    char *url = mg_mprintf("%s/api/v1/rooms/members?room_id=%d",
                           env->backend_url, room_id);
    t_response *response = mx_fetch(url, MX_HTTP_METHOD_GET,
                                    mx_headers_push_back_token(NULL), NULL);

    mx_parse_array_server_response(response,
                                   (t_func_parser)mx_member_parse_cjson);
    mx_strdel(&url);

    return response;
}
t_response *mx_sdk_room_member_post(int room_id, int user_id, bool is_admin) {
    t_sdk_env *env = mx_sdk_env_get();
    char *url = mg_mprintf("%s/api/v1/rooms/members", env->backend_url);
    char *body = mx_room_member_create_stringify(room_id, user_id, is_admin);
    t_response *response = mx_fetch(url, MX_HTTP_METHOD_POST,
                                    mx_headers_push_back_token(NULL), body);

    mx_parse_server_response(response, (t_func_parser)mx_member_parse_cjson);
    mx_strdel(&url);

    return response;
}

t_response *mx_sdk_room_member_put(int member_id, bool is_admin) {
    t_sdk_env *env = mx_sdk_env_get();
    char *url = mg_mprintf("%s/api/v1/rooms/members?member_id=%d",
                           env->backend_url, member_id);
    char *body = mx_room_member_update_stringify(is_admin);
    t_response *response = mx_fetch(url, MX_HTTP_METHOD_PUT,
                                    mx_headers_push_back_token(NULL), body);

    mx_parse_server_response(response, (t_func_parser)mx_member_parse_cjson);
    mx_strdel(&url);

    return response;
}
t_response *mx_sdk_room_member_delete(int member_id) {
    t_sdk_env *env = mx_sdk_env_get();
    char *url = mg_mprintf("%s/api/v1/rooms/members?member_id=%d",
                           env->backend_url, member_id);
    t_response *response = mx_fetch(url, MX_HTTP_METHOD_DELETE,
                                    mx_headers_push_back_token(NULL), NULL);

    mx_parse_server_response(response, (t_func_parser)mx_member_parse_cjson);
    mx_strdel(&url);

    return response;
}

t_response *mx_sdk_room_pined_get(int room_id) {
    t_sdk_env *env = mx_sdk_env_get();
    char *url = mg_mprintf("%s/api/v1/rooms/pined?room_id=%d",
                           env->backend_url, room_id);
    t_response *response = mx_fetch(url, MX_HTTP_METHOD_GET,
                                    mx_headers_push_back_token(NULL), NULL);

    mx_parse_array_server_response(response,
                                   (t_func_parser)mx_pined_parse_cjson);
    mx_strdel(&url);
    return response;
}

t_response *mx_sdk_room_pined_post(int room_id, int message_id) {
    t_sdk_env *env = mx_sdk_env_get();
    char *url = mg_mprintf("%s/api/v1/rooms/pined?room_id=%d&message_id=%d",
                           env->backend_url, room_id, message_id);
    t_response *response = mx_fetch(url, MX_HTTP_METHOD_POST,
                                    mx_headers_push_back_token(NULL), mx_strdup(MX_EMPTY));

    mx_parse_server_response(response, (t_func_parser)mx_pined_parse_cjson);
    mx_strdel(&url);

    return response;
}
t_response *mx_sdk_room_pined_delete(int pined_id) {
    t_sdk_env *env = mx_sdk_env_get();
    char *url = mg_mprintf("%s/api/v1/rooms/pined?pined_id=%d",
                           env->backend_url, pined_id);
    t_response *response = mx_fetch(url, MX_HTTP_METHOD_DELETE,
                                    mx_headers_push_back_token(NULL), NULL);

    mx_parse_server_response(response, (t_func_parser)mx_pined_parse_cjson);
    mx_strdel(&url);

    return response;
}
