#include "../../inc/client.h"

static void on_login_entry_changed(GtkEntry *entry) {
    const gchar *text = gtk_entry_get_text(entry);
    printf("Login: %s\n", text);
}

static void on_password_entry_changed(GtkEntry *entry) {
    const gchar *text = gtk_entry_get_text(entry);
    printf("Password: %s\n", text);
}

static void on_password_entry_visibility(GtkEntry *entry, GtkEntryIconPosition icon_position) {
    if (icon_position == GTK_ENTRY_ICON_SECONDARY) {
        gboolean current_visibility = gtk_entry_get_visibility(entry);
        gtk_entry_set_visibility(entry, !current_visibility);
    }
}

void show_auth_container(GtkBuilder *builder, GtkWidget *window) {
    GtkWidget *auth_container =
        GTK_WIDGET(gtk_builder_get_object(builder, "auth_container"));

    if (auth_container == NULL) {
        g_print("Error: %s\n", "Failed to load auth_container");
        return;
    }

    gtk_container_add(GTK_CONTAINER(window), auth_container);

    GtkWidget *login_entry =
        GTK_WIDGET(gtk_builder_get_object(builder, "login_entry"));
    GtkWidget *password_entry =
        GTK_WIDGET(gtk_builder_get_object(builder, "password_entry"));
    GtkWidget *new_password_entry =
        GTK_WIDGET(gtk_builder_get_object(builder, "new_password_entry"));
    GtkWidget *confirm_password_entry =
        GTK_WIDGET(gtk_builder_get_object(builder, "confirm_password_entry"));

    g_signal_connect(login_entry, "changed",
                     G_CALLBACK(on_login_entry_changed), NULL);
    g_signal_connect(password_entry, "changed",
                     G_CALLBACK(on_password_entry_changed), NULL);
    g_signal_connect(password_entry, "icon-press", 
                    G_CALLBACK(on_password_entry_visibility), NULL);
    g_signal_connect(new_password_entry, "icon-press", 
                    G_CALLBACK(on_password_entry_visibility), NULL);
    g_signal_connect(confirm_password_entry, "icon-press", 
                    G_CALLBACK(on_password_entry_visibility), NULL);
}
