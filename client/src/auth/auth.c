#include "../../inc/client.h"

static void on_login_entry_changed(GtkEntry *entry) {
    const gchar *text = gtk_entry_get_text(entry);
    printf("Login: %s\n", text);
}

static void on_password_entry_changed(GtkEntry *entry) {
    const gchar *text = gtk_entry_get_text(entry);
    printf("Password: %s\n", text);
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
    GtkWidget *password_enrty =
        GTK_WIDGET(gtk_builder_get_object(builder, "password_entry"));

    g_signal_connect(login_entry, "changed",
                     G_CALLBACK(on_login_entry_changed), NULL);
    g_signal_connect(password_enrty, "changed",
                     G_CALLBACK(on_password_entry_changed), NULL);
}
