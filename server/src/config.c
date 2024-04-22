#include "server.h"

static t_env_params *env = NULL;

void mx_usage(t_string program_name) {
    fprintf(stderr,
            "usage: %s [port]\n"
            "  --root ROOT_DIR - root directory, default: '%s'\n"
            "  --log LEVEL    - debug level, from 0 to 4, default: %d\n"
            "  --secret SECRET   - jwt auth secret, default: random uiid\n"
            "  --db DB_PATH   - path to db, default: %s\n"
            "  --domain DOMAIN   - server domain, default: localhost\n",
            program_name, MX_DEFAULT_ROOT_DIR, MG_LL_INFO, MX_DEFAULT_DB_PATH);

    exit(EXIT_FAILURE);
}

t_env_params *mx_env_get(void) {
    return env;
}

void mx_env_set(t_env_params *e) {
    env = e;
}

t_env_params *mx_create_env() {
    t_env_params *env = malloc(sizeof(t_env_params));

    env->port = 3000;
    env->log_level = MG_LL_INFO;
    env->jwt_auth_secret = mx_gen_uuid();
    env->root_dir = mx_strdup(MX_DEFAULT_ROOT_DIR);
    env->domain = mx_strdup("localhost");
    env->db_connection = NULL;
    env->db_path = NULL;
    env->upload_dir = NULL;

    return env;
}

static void read_env_params(t_env_params *env, int argc, char *argv[]) {
    if (argc < 2) {
        mx_usage(argv[0]);
    }
    int port = atoi(argv[1]);

    if (port > 0 && port < 65535) {
        env->port = port;
    } else {
        mx_usage(argv[0]);
    }

    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            mx_usage(argv[0]);
        } else if (strcmp(argv[i], "--log") == 0) {
            env->log_level = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--root") == 0) {
            free(env->root_dir);
            env->root_dir = mx_strdup(argv[++i]);
        } else if (strcmp(argv[i], "--secret") == 0) {
            free(env->jwt_auth_secret);
            env->jwt_auth_secret = mx_strdup(argv[++i]);
        } else if (strcmp(argv[i], "--db") == 0) {
            free(env->db_path);
            env->db_path = mx_strdup(argv[++i]);
        } else if (strcmp(argv[i], "--domain") == 0) {
            free(env->domain);
            env->domain = mx_strdup(argv[++i]);
        } else {
            mx_usage(argv[0]);
        }
    }
}

void mx_env_init(t_env_params *env, int argc, char *argv[]) {
    srand(time(NULL));
    read_env_params(env, argc, argv);

    if (!env->db_path) {
        env->db_path = mx_path_join(env->root_dir, MX_DEFAULT_DB_PATH);
    }

    env->upload_dir = mx_path_join(env->root_dir, MX_UPLOAD_DIR_PATH);
    struct mg_http_serve_opts opt = {
        .root_dir = mg_mprintf("/assets=%s/%s", env->root_dir, MX_STATIC_DIR),
        .extra_headers = MX_OPTIONS_HEADERS};
    env->static_dir_opt = opt;
    env->db_connection = mx_connect_to_database(env->db_path);
    sqlite3_exec(env->db_connection, "PRAGMA foreign_keys = ON;", NULL, NULL,
                 NULL);
    printf("Upload directory: %s\n", env->upload_dir);
    mx_create_path_if_not_exist(env->upload_dir);
}

void mx_env_close(t_env_params *env) {
    mx_strdel(&env->root_dir);
    mx_strdel(&env->domain);
    mx_strdel(&env->jwt_auth_secret);
    mx_strdel(&env->db_path);
    mx_strdel(&env->upload_dir);
    mx_strdel((char **)&env->static_dir_opt.root_dir);
    sqlite3_close(env->db_connection);

    free(env);
}
