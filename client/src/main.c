#define DEFINE_GLOBALS
#include "client.h"

static void usage(char *program_name) {
    g_print("usage: %s [server ip] [server port]\n", program_name);
    exit(EXIT_FAILURE);
}

static t_string parse_params(int argc, char *argv[]) {
    if (argc != 3) {
        usage(argv[0]);
    }
    int port = atoi(argv[2]);

    if (port <= 0 || port > 65535) {
        usage(argv[0]);
    }

    return mg_mprintf("%s:%s", argv[1], mx_itoa(port));
}

int main(int argc, char *argv[]) {
    t_string addr = parse_params(argc, argv);
    t_string http_server_url = mg_mprintf("http://%s", addr);
    t_string ws_server_url = mg_mprintf("ws://%s/ws", addr);

    gtk_init(&argc, &argv);
    mx_sdk_init(http_server_url);

    global_builder = gtk_builder_new();

    if (gtk_builder_add_from_file(global_builder, "client/static/uchat.glade",
                                  NULL) == 0) {
        g_print("Error: %s\n", "Failed to load glade file");
        return EXIT_FAILURE;
    }

    global_window =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "main_window"));

    if (global_window == NULL) {
        g_print("Error: %s\n", "Failed to load auth_window");
        return EXIT_FAILURE;
    }
    gtk_window_set_title(GTK_WINDOW(global_window), "Cozy Chat");

    load_css(global_window, "client/static/styles/light.css");
    load_css(global_window, "client/static/styles/style.css");

    gtk_builder_connect_signals(global_builder, NULL);
    g_signal_connect(global_window, "destroy", G_CALLBACK(gtk_main_quit),
                     NULL);
    init_theme_switcher(global_builder, global_window, "theme_switcher");
    show_auth_container();
    gtk_widget_show_all(global_window);
    ws_client_init(ws_server_url);
    gtk_main();
    mx_sdk_free();
    ws_client_free();
    mx_strdel(&addr);
    mx_strdel(&http_server_url);
    mx_strdel(&ws_server_url);

    return EXIT_SUCCESS;
}
