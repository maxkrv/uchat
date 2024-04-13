#include "../../inc/client.h"

char *new_password = NULL;
char *old_password = NULL;

static void on_old_password_entry_changed(GtkEntry *entry) {
    const gchar *text = gtk_entry_get_text(entry);
    old_password = g_strdup(text);
}

static void on_new_password_entry_changed(GtkEntry *entry) {
    const gchar *text = gtk_entry_get_text(entry);
    new_password = g_strdup(text);
}

static void on_password_entry_visibility(GtkEntry *entry,
                                         GtkEntryIconPosition icon_position) {
    if (icon_position == GTK_ENTRY_ICON_SECONDARY) {
        gboolean current_visibility = gtk_entry_get_visibility(entry);
        gtk_entry_set_visibility(entry, !current_visibility);
    }
}

static void on_update_password_button_clicked() {
    t_response *response = mx_sdk_change_password(old_password, new_password);

    if (mx_is_response_error(response)) {
        g_print("Error: %s\n", mx_sdk_exception_get_message(response));

        mx_sdk_response_free(response, free);
        return;
    }

    mx_sdk_response_free(response, (t_func_free)mx_user_free);
    clean_up_settings();
    new_password = NULL;
    old_password = NULL;
}

void init_update_password_form() {
    GtkWidget *old_password_entry = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "old_password_entry"));
    GtkWidget *new_password_entry = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "new_update_password_entry"));
    GtkWidget *update_password_button = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "update_password_button"));

    g_signal_connect(old_password_entry, "changed",
                     G_CALLBACK(on_old_password_entry_changed), NULL);
    g_signal_connect(new_password_entry, "changed",
                     G_CALLBACK(on_new_password_entry_changed), NULL);
    g_signal_connect(update_password_button, "clicked",
                     G_CALLBACK(on_update_password_button_clicked), NULL);
    g_signal_connect(old_password_entry, "icon-press",
                     G_CALLBACK(on_password_entry_visibility), NULL);
    g_signal_connect(new_password_entry, "icon-press",
                     G_CALLBACK(on_password_entry_visibility), NULL);
}
