#include "../../inc/client.h"

GtkBuilder *chat_global_builder;
GtkWidget *chat_global_window;

// could be moved for a better solution in the future
static void populate_chat_side_bar(t_user *user) {
    GtkWidget *chat_side_bar = GTK_WIDGET(
        gtk_builder_get_object(chat_global_builder, "chat_sidebar"));

    if (chat_side_bar == NULL) {
        g_print("Error: %s\n", "Failed to load chat_side_bar");
        return;
    }

    GtkWidget *user_name =
        GTK_WIDGET(gtk_builder_get_object(chat_global_builder, "user_name"));

    if (user_name == NULL) {
        g_print("Error: %s\n", "Failed to load user_name");
        return;
    }

    gtk_label_set_text(GTK_LABEL(user_name), user->name);
}

void show_chat_container(GtkBuilder *builder, GtkWidget *window) {
    chat_global_builder = builder;
    chat_global_window = window;
    GtkWidget *chat_container =
        GTK_WIDGET(gtk_builder_get_object(builder, "chat_container"));

    if (chat_container == NULL) {
        g_print("Error: %s\n", "Failed to load chat_container");
        return;
    }

    t_response *response = mx_sdk_user_get_me();

    if (mx_is_response_error(response)) {
        g_print("Error: %s\n", response->exception->message);
        return;
    }

    t_user *user = mx_entity_parse_string(response->body,
                                          (t_func_parser)mx_user_parse_cjson);

    populate_chat_side_bar(user);

    gtk_container_add(GTK_CONTAINER(window), chat_container);
    gtk_widget_show(chat_container);
}

void hide_chat_container() {
    GtkWidget *chat_container = GTK_WIDGET(
        gtk_builder_get_object(chat_global_builder, "chat_container"));

    if (chat_container == NULL) {
        g_print("Error: %s\n", "Failed to load chat_container");
        return;
    }

    gtk_container_remove(GTK_CONTAINER(chat_global_window), chat_container);
}
