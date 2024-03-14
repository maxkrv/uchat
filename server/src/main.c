#include "server.h"

int main(int argc, char *argv[]) {
    srand(time(NULL));

    t_env_params *env = mx_create_env();

    mx_init_env(env, argc, argv);

    t_string port = mx_itoa(env->port);
    t_string addr = mx_strjoin("http://localhost:", port);

    mx_run_migrations(env->db_connection, env);
    mx_run_server(env, addr);

    mx_strdel(&addr);
    mx_strdel(&port);
    mx_close_env(env);

    return 0;
}
