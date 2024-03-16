#include "auth.h"

t_jwt_token mx_create_auth_jwt(int user_id) {
    cJSON *json = cJSON_CreateObject();
    t_env_params *env = mx_env_get();

    cJSON_AddNumberToObject(json, "user_id", user_id);

    char *payload = cJSON_PrintUnformatted(json);

    cJSON_Delete(json);

    t_jwt_token token =
        mx_create_jwt(payload, env->jwt_auth_secret, MX_AUTH_JWT_EXP_TIME);

    mx_strdel(&payload);

    return token;
}

t_user_id mx_verify_auth_jwt(char *jwt) {
    t_env_params *env = mx_env_get();
    char *payload = mx_verify_jwt(jwt, env->jwt_auth_secret);

    if (!payload) {
        return -1;
    }

    cJSON *json = cJSON_Parse(payload);

    if (!json) {
        mx_strdel(&payload);

        return -1;
    }

    t_user_id user_id = cJSON_GetNumberValue(
        cJSON_GetObjectItemCaseSensitive(json, "user_id"));

    mx_strdel(&payload);
    cJSON_Delete(json);

    return user_id;
}
