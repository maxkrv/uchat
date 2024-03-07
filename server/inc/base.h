#ifndef UCHAT_BASE
#define UCHAT_BASE

#define MX_BUFFER_SIZE 8192
#define MX_EMPTY ""

typedef struct mg_mgr t_mg_manager;
typedef struct mg_http_message t_http_message;
typedef struct mg_connection t_connection;
typedef char *t_string;

#include <signal.h>
#include "cJSON.h"
#include "mongoose.h"
#include "sqlite3.h"
#include "libmx.h"
#include "http.h"

typedef struct s_env_params {
    int port;
    char *root_dir;
    int log_level;
    char *jwt_auth_secret;
} t_env_params;

t_env_params *mx_get_env(void);

#endif
