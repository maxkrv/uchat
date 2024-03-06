#include "client.h"

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkBuilder *builder = gtk_builder_new();

    if (gtk_builder_add_from_file(builder, "client/static/uchat.glade",
                                  NULL) == 0) {
        g_print("Error: %s\n", "Failed to load glade file");
        return 1;
    }

    GtkWidget *window =
        GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));

    if (window == NULL) {
        g_print("Error: %s\n", "Failed to load auth_window");
        return 1;
    }

    load_css(window, "client/static/styles/light.css");
    load_css(window, "client/static/styles/style.css");

    gtk_builder_connect_signals(builder, NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    init_theme_switcher(builder, window);

    show_auth_container(builder, window);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
