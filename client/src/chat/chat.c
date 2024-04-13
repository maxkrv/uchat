#include "client.h"

static void populate_chat_side_bar(bool should_load_rooms) {
    GtkWidget *rooms_list =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "rooms_list"));

    if (should_load_rooms) {
        render_rooms();
    }

    gtk_widget_show_all(rooms_list);
}

void show_chat_container(bool should_load_rooms) {
    GtkWidget *chat_container =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "chat_container"));

    populate_chat_side_bar(should_load_rooms);

    GtkWidget *user_settings_button = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "user_settings_button"));

    g_signal_connect(user_settings_button, "clicked",
                     G_CALLBACK(show_settings_dialog), global_builder);

    init_create_room();

    render_user();

    gtk_container_add(GTK_CONTAINER(global_window), chat_container);
}
