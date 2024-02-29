#include "auth.h"

char *mx_create_auth_jwt(int user_id) {
    cJSON *json = cJSON_CreateObject();
    t_env_params *env = mx_get_env();

    cJSON_AddNumberToObject(json, "user_id", user_id);

    char *payload = cJSON_PrintUnformatted(json);

    cJSON_Delete(json);

    char *token =
        mx_create_jwt(payload, env->jwt_auth_secret, MX_AUTH_JWT_EXP_TIME);

    mx_strdel(&payload);

    return token;
}

int mx_verify_auth_jwt(char *jwt) {
    t_env_params *env = mx_get_env();
    char *payload = mx_verify_jwt(jwt, env->jwt_auth_secret);

    if (!payload) {
        return -1;
    }

    cJSON *json = cJSON_Parse(payload);

    if (!json) {
        mx_strdel(&payload);

        return -1;
    }

    int user_id = cJSON_GetNumberValue(
        cJSON_GetObjectItemCaseSensitive(json, "user_id"));

    mx_strdel(&payload);
    cJSON_Delete(json);

    return user_id;
}
