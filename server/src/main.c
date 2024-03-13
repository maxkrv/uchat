#include "server.h"

static void usage(void) {
    fprintf(stderr,
            "Usage:\n"
            "  --port PORT     - listening port, default: %d\n"
            "  --root ROOT_DIR - root directory, default: '%s'\n"
            "  --log LEVEL    - debug level, from 0 to 4, default: %d\n"
            "  --secret SECRET   - jwt auth secret, default: random uiid\n"
            "  --db DB_PATH   - path to db, default: %s\n"
            "  --domain DOMAIN   - server domain, default: localhost\n",
            3000, MX_DEFAULT_ROOT_DIR, MG_LL_INFO, MX_DEFAULT_DB_PATH);

    exit(EXIT_FAILURE);
}

static t_env_params *init_env(int argc, char *argv[]) {
    t_env_params *mx_env = mx_get_env();

    mx_env->jwt_auth_secret = mx_gen_uuid();
    mx_env->root_dir = MX_DEFAULT_ROOT_DIR;
    mx_env->domain = "localhost";

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--log") == 0) {
            mx_env->log_level = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--port") == 0) {
            mx_env->port = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--root") == 0) {
            mx_env->root_dir = argv[++i];
        } else if (strcmp(argv[i], "--secret") == 0) {
            mx_env->jwt_auth_secret = argv[++i];
        } else if (strcmp(argv[i], "--db") == 0) {
            mx_env->db_path = argv[++i];
        } else if (strcmp(argv[i], "--domain") == 0) {
            mx_env->domain = argv[++i];
        } else {
            usage();
        }
    }
    if (!mx_env->db_path) {
        char *tmp = mx_strjoin(mx_env->root_dir, "/");
        mx_env->db_path = mx_strjoin(tmp, MX_DEFAULT_DB_PATH);

        mx_strdel(&tmp);
    }

    mx_env->upload_dir =
        mx_path_join(mx_get_env()->root_dir, MX_UPLOAD_DIR_PATH);
    char *tmp = mx_path_join(mx_get_env()->root_dir, MX_STATIC_DIR);

    mx_env->static_dir_opt = mg_mprintf("/%s=%s", MX_STATIC_DIR, tmp);
    mx_env->db_connection = mx_connect_to_database(mx_env->db_path);

    mx_create_path_if_not_exist(mx_env->upload_dir);

    mx_strdel(&tmp);
    return mx_env;
}

int main(int argc, char *argv[]) {
    srand(time(NULL));

    t_env_params *mx_env = init_env(argc, argv);
    char *addr = mx_strjoin("http://localhost:", mx_itoa(mx_env->port));

    mx_run_migrations(mx_env->db_connection);
    mx_run_server(mx_env, addr);

    free(addr);

    return 0;
}
