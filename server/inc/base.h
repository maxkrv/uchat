#ifndef UCHAT_BASE
#define UCHAT_BASE

#include <signal.h>
#include "mongoose.h"
#include "libmx.h"

typedef struct mg_mgr t_mg_manager;
typedef struct mg_http_message t_http_message;
typedef struct mg_connection t_connection;

typedef struct s_env_params {
    int port;
    char *root_dir;
    int log_level;
} t_env_params;

t_env_params mx_get_env(void);

#endif
