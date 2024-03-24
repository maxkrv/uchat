#ifndef SDK_BASE_H
#define SDK_BASE_H

#include "unistd.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"
#include "cJSON.h"
#include "string.h"
#include "mongoose.h"
#include <curl/curl.h>
#include "libmx.h"

#define MX_BUFFER_SIZE 8192
#define MX_EMPTY ""
#define cJSON_GetKey cJSON_GetObjectItemCaseSensitive
typedef char *t_string;
typedef void *(*t_func_parser)(cJSON *json);
typedef void (*t_func_free)(void *);
typedef struct {
    char *backend_url;
    char *jwt_token;
    char *username;
    char *password;
} t_sdk_env;

t_sdk_env *mx_sdk_env_init(void);
t_sdk_env *mx_sdk_env_get(void);
void mx_sdk_env_free(t_sdk_env *);
void mx_dk_free_jwt_token(void);
void mx_sdk_env_set(t_sdk_env *);

#endif
