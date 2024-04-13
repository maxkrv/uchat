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

    t_response *rooms_response = mx_sdk_rooms_get();

    if (mx_is_response_error(rooms_response)) {
        g_print("Error: %s\n", mx_sdk_exception_get_message(rooms_response));
        mx_sdk_response_free(rooms_response, (t_func_free)mx_room_free);
        return;
    }

    global_rooms = rooms_response->data;

    populate_chat_side_bar(should_load_rooms);

    GtkWidget *user_settings_button = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "user_settings_button"));

    g_signal_connect(user_settings_button, "clicked",
                     G_CALLBACK(show_settings_dialog), global_builder);

    init_create_room();

    GtkWidget *room_settings_button = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "room_settings_button"));

    g_signal_connect(room_settings_button, "clicked",
                     G_CALLBACK(show_room_settings_dialog), global_builder);

    render_user();

    gtk_container_add(GTK_CONTAINER(global_window), chat_container);
}
