#include "../../inc/client.h"

static void on_cancel_room_settings(GtkButton *button, gpointer user_data) {
    GtkWidget *dialog = GTK_WIDGET(user_data);
    gtk_widget_hide(dialog);
    (void)button;
}

void show_room_settings_dialog(GtkButton *button, gpointer user_data) {
    GtkBuilder *builder = GTK_BUILDER(user_data);
    GtkWidget *dialog = GTK_WIDGET(
        gtk_builder_get_object(builder, "room_settings_dialog"));
    GtkWidget *cancel_button = GTK_WIDGET(
        gtk_builder_get_object(builder, "room_settings_cancel"));

    gtk_window_set_transient_for(
        GTK_WINDOW(dialog),
        GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))));
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ON_PARENT);

    g_signal_connect(cancel_button, "clicked",
        G_CALLBACK(on_cancel_room_settings), dialog);

    gint response = gtk_dialog_run(GTK_DIALOG(dialog));

    if (response == GTK_RESPONSE_ACCEPT) {
        g_print("OK\n");
    } else if (response == GTK_RESPONSE_CANCEL) {
        g_print("Cancel\n");
    }
}
