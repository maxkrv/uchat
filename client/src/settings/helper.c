#include "client.h"

void clean_up_settings() {
    hide_settings_dialog();

    GtkWidget *username_entry =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "username_entry"));
    GtkWidget *tag_entry =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "tag_entry"));
    GtkWidget *description_entry = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "description_entry"));
    GtkWidget *avatar_button =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "avatar_button"));

    GtkWidget *old_password_entry = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "old_password_entry"));
    GtkWidget *new_password_entry = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "new_update_password_entry"));

    gtk_entry_set_text(GTK_ENTRY(username_entry), "");
    gtk_entry_set_text(GTK_ENTRY(tag_entry), "");
    gtk_entry_set_text(GTK_ENTRY(description_entry), "");
    gtk_entry_set_text(GTK_ENTRY(old_password_entry), "");
    gtk_entry_set_text(GTK_ENTRY(new_password_entry), "");
    gtk_file_chooser_unselect_all(GTK_FILE_CHOOSER(avatar_button));

    render_user();
}

