#include "sdk/sdk.h"

static t_sdk_env *sdk_env = NULL;

t_sdk_env *mx_sdk_env_init(void) {
    t_sdk_env *env = malloc(sizeof(t_sdk_env));

    env->backend_url = NULL;
    env->jwt_token = NULL;
    env->password = NULL;
    env->username = NULL;

    return env;
}

void mx_sdk_env_free(t_sdk_env *env) {
    mx_strdel(&env->backend_url);
    mx_strdel(&env->jwt_token);
    mx_strdel(&env->password);
    mx_strdel(&env->username);

    free(env);
}

void mx_dk_free_jwt_token(void) {
    mx_strdel(&sdk_env->jwt_token);
    mx_strdel(&sdk_env->username);
    mx_strdel(&sdk_env->password);
}

t_sdk_env *mx_sdk_env_get(void) {
    return sdk_env;
}

void mx_sdk_env_set(t_sdk_env *env) {
    sdk_env = env;
}
