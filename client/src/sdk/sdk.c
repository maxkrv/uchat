#include "sdk/sdk.h"

/* Backend url like "http://localhost:3000"*/
void mx_sdk_init(const char *backend_url) {
    t_sdk_env *env = mx_sdk_env_get() ? mx_sdk_env_get() : mx_sdk_env_init();

    mx_strdel(&env->backend_url);
    env->backend_url = mx_strdup(backend_url);

    mx_sdk_env_set(env);
    curl_global_init(CURL_GLOBAL_ALL);
    return;
}

void mx_sdk_free(void) {
    mx_sdk_env_free(mx_sdk_env_get());
    mx_sdk_env_set(NULL);

    curl_global_cleanup();
}
