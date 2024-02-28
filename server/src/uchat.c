#include <server.h>

static int sig_no;

t_env_params mx_get_env(void) {
    static t_env_params env = {3000, ".", MG_LL_INFO};

    return env;
}

static void signal_handler(int i) {
    sig_no = i;
}

int mx_run_server(t_env_params *env, char *addr) {
    t_mg_manager mgr;

    mg_mgr_init(&mgr);
    mg_log_set(env->log_level);
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    if (!mg_http_listen(&mgr, addr, mx_http_request_handler, NULL)) {
        MG_ERROR(("Cannot listen on %d. Use another PORT", env->port));
        exit(EXIT_FAILURE);
    }
    MG_INFO(("Listening on     : %s", addr));

    while (sig_no == 0) {
        mg_mgr_poll(&mgr, 1000);
    }

    mg_mgr_free(&mgr);
    MG_INFO(("Exiting on signal %d", sig_no));

    return EXIT_SUCCESS;
}
