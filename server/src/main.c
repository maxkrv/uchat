#include <server.h>

static void usage(void) {
    t_env_params mx_env = mx_get_env();
    fprintf(stderr,
            "Usage:\n"
            "  -p PORT     - listening port, default: %d\n"
            "  -d ROOT_DIR - root directory, default: '%s'\n"
            "  -l LEVEL    - debug level, from 0 to 4, default: %d\n",
            mx_env.port, mx_env.root_dir, mx_env.log_level);

    exit(EXIT_FAILURE);
}

static t_env_params init_env(int argc, char *argv[]) {
    t_env_params mx_env = mx_get_env();

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-l") == 0) {
            mx_env.log_level = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-p") == 0) {
            mx_env.port = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-d") == 0) {
            mx_env.root_dir = argv[++i];
        } else {
            usage();
        }
    }

    return mx_env;
}

// todo: ./user is structure example for every module with router inside
int main(int argc, char *argv[]) {
    t_env_params mx_env = init_env(argc, argv);
    char *addr = mx_strjoin("http://localhost:", mx_itoa(mx_env.port));

    mx_run_server(&mx_env, addr);

    free(addr);

    return 0;
}
