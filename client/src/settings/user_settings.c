#include "../../inc/client.h"

static void back_to_chat_cb(void) {
    hide_settings_container();

    show_chat_container(false);
}

void on_options_button_clicked(void) {
    GtkWidget *popover =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "logoout_popover"));

    gtk_popover_popup(GTK_POPOVER(popover));
}

void on_logout_button_clicked(void) {
    mx_sdk_logout();
    hide_settings_container();
    show_auth_container();
}

void hide_settings_container(void) {
    GtkWidget *settings_container = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "user_settings_container"));

    if (settings_container == NULL) {
        g_print("Error: %s\n", "Failed to load settings_container");
        return;
    }

    gtk_container_remove(GTK_CONTAINER(global_window), settings_container);
}

void show_settings_container(void) {
    GtkWidget *settings_container = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "user_settings_container"));

    if (settings_container == NULL) {
        g_print("Error: %s\n", "Failed to load settings_container");
        return;
    }
    init_theme_switcher(global_builder, global_window, "theme_switcher1");

    GtkWidget *back_button =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "back_button"));
    GtkWidget *options_button =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "options_button"));
    GtkWidget *logout_button =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "logout_button"));

    g_signal_connect(back_button, "clicked", G_CALLBACK(back_to_chat_cb),
                     NULL);
    g_signal_connect(options_button, "clicked",
                     G_CALLBACK(on_options_button_clicked), NULL);
    g_signal_connect(logout_button, "clicked",
                     G_CALLBACK(on_logout_button_clicked), NULL);

    gtk_container_add(GTK_CONTAINER(global_window), settings_container);

    init_update_user_form_field();
    init_update_password_form();
}
