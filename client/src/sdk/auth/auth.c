#include "sdk/sdk.h"

static char *login_dto_stringify(const char *name, const char *password) {
    cJSON *body = cJSON_CreateObject();

    cJSON_AddStringToObject(body, "name", name);
    cJSON_AddStringToObject(body, "password", password);

    char *body_str = cJSON_PrintUnformatted(body);
    cJSON_Delete(body);

    return body_str;
}
static char *change_password_dto_stringify(const char *old_password,
                                           const char *new_password) {
    cJSON *body = cJSON_CreateObject();

    cJSON_AddStringToObject(body, "old_password", old_password);
    cJSON_AddStringToObject(body, "new_password", new_password);

    char *body_str = cJSON_PrintUnformatted(body);
    cJSON_Delete(body);

    return body_str;
}
static void *parse_token(cJSON *response_json) {
    return mx_cjson_get_string(response_json, "token");
}

t_response *mx_sdk_login(const char *name, const char *password) {
    t_sdk_env *env = mx_sdk_env_get();
    char *url = mg_mprintf("%s/api/v1/auth/login", env->backend_url);
    char *body_str = login_dto_stringify(name, password);
    t_response *response = mx_fetch(url, MX_HTTP_METHOD_POST, NULL, body_str);

    mx_parse_server_response(response, parse_token);

    if (response && response->data) {
        free(env->jwt_token);
        free(env->password);
        free(env->username);
        env->jwt_token = mx_strdup(response->data);
        env->password = mx_strdup(password);
        env->username = mx_strdup(name);
    }

    mx_strdel(&url);

    return response;
}

bool mx_sdk_logout(void) {
    t_sdk_env *env = mx_sdk_env_get();

    if (!env) {
        return false;
    }
    free(env->jwt_token);
    free(env->password);
    free(env->username);
    env->jwt_token = NULL;
    env->password = NULL;
    env->username = NULL;

    return true;
}

t_response *mx_sdk_refresh_token(void) {
    t_sdk_env *env = mx_sdk_env_get();

    if (!env || !env->password || !env->username) {
        return NULL;
    }

    char *url = mg_mprintf("%s/api/v1/auth/login", env->backend_url);
    char *body_str = login_dto_stringify(env->username, env->password);
    t_response *response = mx_fetch(url, MX_HTTP_METHOD_POST, NULL, body_str);

    mx_parse_server_response(response, parse_token);

    if (response->data) {
        free(env->jwt_token);
        env->jwt_token = mx_strdup(response->data);
    }

    mx_strdel(&url);

    return response;
}

t_response *mx_sdk_change_password(const char *old_password,
                                   const char *new_password) {
    t_sdk_env *env = mx_sdk_env_get();
    char *url = mg_mprintf("%s/api/v1/auth/change/password", env->backend_url);
    char *body_str = change_password_dto_stringify(old_password, new_password);
    t_response *response = mx_fetch(
        url, MX_HTTP_METHOD_POST, mx_headers_push_back_token(NULL), body_str);

    mx_parse_server_response(response, (t_func_parser)mx_user_parse_cjson);
    mx_strdel(&url);

    return response;
}

t_response *mx_sdk_register(t_user_create_dto *dto) {
    t_sdk_env *env = mx_sdk_env_get();
    char *url = mg_mprintf("%s/api/v1/auth/register", env->backend_url);
    char *body_str = mx_user_create_dto_stringify(dto);
    t_response *response = mx_fetch(
        url, MX_HTTP_METHOD_POST, mx_headers_push_back_token(NULL), body_str);

    mx_user_create_dto_free(dto);
    mx_parse_server_response(response, (t_func_parser)mx_user_parse_cjson);
    mx_strdel(&url);

    return response;
}
