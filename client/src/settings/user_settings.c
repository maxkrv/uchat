#include "../../inc/client.h"

static void on_cancel_user_settings(GtkButton *button, gpointer user_data) {
    GtkWidget *settings_dialog = GTK_WIDGET(user_data);
    gtk_widget_hide(settings_dialog);
    (void)button;
}

void on_options_button_clicked(void) {
    GtkWidget *popover =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "logoout_popover"));

    gtk_popover_popup(GTK_POPOVER(popover));
}

void on_logout_button_clicked(void) {
    mx_sdk_logout();
    hide_settings_dialog();
    show_auth_container();
}

void hide_settings_dialog(void) {
    GtkWidget *settings_dialog = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "user_settings_dialog"));

    if (settings_dialog == NULL) {
        g_print("Error: %s\n", "Failed to load settings_dialog");
        return;
    }

    gtk_widget_hide(GTK_WIDGET(settings_dialog));
}

void show_settings_dialog(GtkButton *button, gpointer user_data) {
    GtkBuilder *builder = GTK_BUILDER(user_data);
    GtkDialog *settings_dialog =
        GTK_DIALOG(gtk_builder_get_object(builder, "user_settings_dialog"));
    GtkWidget *cancel_button =
        GTK_WIDGET(gtk_builder_get_object(builder, "user_settings_cancel"));

    if (settings_dialog == NULL) {
        g_print("Error: %s\n", "Failed to load user_settings_dialog");
        return;
    }
    init_theme_switcher(global_builder, GTK_WIDGET(settings_dialog),
                        "theme_switcher1");

    GtkWidget *options_button =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "options_button"));
    GtkWidget *logout_button =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "logout_button"));

    gtk_window_set_transient_for(
        GTK_WINDOW(settings_dialog),
        GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))));
    gtk_window_set_position(GTK_WINDOW(settings_dialog),
                            GTK_WIN_POS_CENTER_ON_PARENT);

    g_signal_connect(cancel_button, "clicked",
                     G_CALLBACK(on_cancel_user_settings), settings_dialog);
    g_signal_connect(options_button, "clicked",
                     G_CALLBACK(on_options_button_clicked), NULL);
    g_signal_connect(logout_button, "clicked",
                     G_CALLBACK(on_logout_button_clicked), NULL);
    init_update_user_form_field();
    init_update_password_form();

    gint response = gtk_dialog_run(GTK_DIALOG(settings_dialog));

    if (response == GTK_RESPONSE_ACCEPT) {
        g_print("OK\n");
        // Perform actions for OK button
    } else if (response == GTK_RESPONSE_CANCEL) {
        g_print("Cancel\n");
        // Perform actions for Cancel button
    }
}
