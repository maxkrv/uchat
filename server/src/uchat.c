#include <server.h>

static int sig_no;

static void signal_handler(int i) {
    sig_no = i;
}

int mx_run_server(t_env_params *env, t_string addr) {
    t_mg_manager mgr;

    mx_env_set(env);
    mx_ws_server_set(mx_ws_server_init());
    mg_mgr_init(&mgr);
    mg_log_set(env->log_level);
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    if (!mg_http_listen(&mgr, addr, mx_event_handler, NULL)) {
        MG_ERROR(("Cannot listen on %d. Use another PORT", env->port));
        exit(EXIT_FAILURE);
    }
    MG_INFO(("Listening on: %s", addr));

    while (sig_no == 0) {
        mg_mgr_poll(&mgr, 1000);
    }

    mg_mgr_free(&mgr);
    mx_ws_server_free(mx_ws_server_get());
    mx_ws_server_set(NULL);
    MG_INFO(("Exiting on signal %d", sig_no));

    return EXIT_SUCCESS;
}
