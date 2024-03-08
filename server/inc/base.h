#ifndef UCHAT_BASE
#define UCHAT_BASE

#define MX_BUFFER_SIZE 8192
#define MX_EMPTY ""
#define MX_DEFAULT_DB_PATH "db/sqlite.db"
#define MX_DEFAULT_ROOT_DIR "."

typedef struct mg_mgr t_mg_manager;
typedef struct mg_http_message t_http_message;
typedef struct mg_connection t_connection;
typedef char *t_string;

#include <signal.h>
#include "cJSON.h"
#include "mongoose.h"
#include "sqlite3.h"
#include "libmx.h"
#include "dirent.h"
#include "http.h"

#define MX_HEADERS_JSON                                                       \
    "Access-Control-Allow-Origin: *\r\n"                                      \
    "Access-Control-Allow-Methods: *\r\n"                                     \
    "Access-Control-Allow-Headers: *\r\n"                                     \
    "Access-Control-Max-Age: 3600\r\n"                                        \
    "Content-Type: application/json\r\n"
#define MX_OPTIONS_HEADERS                                                    \
    "Access-Control-Allow-Origin: *\r\n"                                      \
    "Access-Control-Allow-Methods: *\r\n"                                     \
    "Access-Control-Allow-Headers: *\r\n"                                     \
    "Access-Control-Max-Age: 3600\r\n"
typedef struct s_env_params {
    int port;
    char *root_dir;
    int log_level;
    char *jwt_auth_secret;
    char *db_path;
    sqlite3 *db_connection;
} t_env_params;

t_env_params *mx_get_env(void);

#endif
