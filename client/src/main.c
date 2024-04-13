#define DEFINE_GLOBALS
#include "client.h"

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    mx_sdk_init("http://localhost:3000");

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

    load_css(global_window, "client/static/styles/light.css");
    load_css(global_window, "client/static/styles/style.css");

    gtk_builder_connect_signals(global_builder, NULL);
    g_signal_connect(global_window, "destroy", G_CALLBACK(gtk_main_quit),
                     NULL);
    init_theme_switcher(global_builder, global_window, "theme_switcher");
    show_auth_container();
    gtk_widget_show_all(global_window);
    ws_client_init("ws://localhost:3000/ws");
    gtk_main();
    mx_sdk_free();
    ws_client_free();

    return EXIT_SUCCESS;
}
