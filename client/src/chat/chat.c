#include "../../inc/client.h"

static void show_settings_container_cb() {
    hide_chat_container();
    show_settings_container();
}

static void set_chat_scrollbar_to_bottom(GtkWidget *scrolled_chat_window) {
    GtkAdjustment *adjustment = gtk_scrolled_window_get_vadjustment(
        GTK_SCROLLED_WINDOW(scrolled_chat_window));
    double upper = gtk_adjustment_get_upper(adjustment);
    double page_size = gtk_adjustment_get_page_size(adjustment);
    gtk_adjustment_set_value(adjustment, upper - page_size);
}

// could be moved for a better solution in the future
static void populate_chat_side_bar(t_user *user) {
    GtkWidget *chat_side_bar = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "chat_sidebar"));

    if (chat_side_bar == NULL) {
        g_print("Error: %s\n", "Failed to load chat_side_bar");
        return;
    }

    GtkWidget *user_name =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "user_name"));

    if (user_name == NULL) {
        g_print("Error: %s\n", "Failed to load user_name");
        return;
    }

    gtk_label_set_text(GTK_LABEL(user_name), user->name);
}

void show_chat_container() {
    GtkWidget *chat_container =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "chat_container"));

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

    GtkWidget *scrolled_window = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "scrolled_chat_window"));

    set_chat_scrollbar_to_bottom(scrolled_window);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), 
                                     GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

    GtkWidget *user_settings_button = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "user_settings_button"));

    if (user_settings_button == NULL) {
        g_print("Error: %s\n", "Failed to load user_settings_button");
    }

    g_signal_connect(user_settings_button, "clicked",
                     G_CALLBACK(show_settings_container_cb), global_builder);

    gtk_container_add(GTK_CONTAINER(global_window), chat_container);
}

void hide_chat_container() {
    GtkWidget *chat_container = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "chat_container"));

    if (chat_container == NULL) {
        g_print("Error: %s\n", "Failed to load chat_container");
        return;
    }

    gtk_container_remove(GTK_CONTAINER(global_window), chat_container);
}
